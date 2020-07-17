// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "Lexer.h"
#include "Parser.h"
#include "ast/nodes.h"
#include "exceptions.h"
#include "NameResolver.h"
#include "TypeChecker.h"
#include "ByteCodeCompiler.h"
#include "Scope.h"
#include "vm/VM.h"
#include "vm/ByteCodeChunk.h"
#include "SourceFile.h"
#include "vm/Debugger.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cstring>

using namespace Sky;

namespace Sky {
	int g_timeout = -1;
	std::string g_homePath;
	std::string g_lookupPath;
	unsigned short g_debugPort = 0;
}

void error(const std::string& msg) {
    std::cerr << "\033[1;31m" << msg << "\033[0m\n";
}

void error(const Sky::Node* n, const std::string& msg) {
    std::cerr << "\033[1;31m" << n->source->filename << ":" << n->line << ":" << n->column << " " << msg << "\033[0m\n";
}

void bail() {
    error("Aborting due to previous errors.");
    exit(1);
}

void help() {
    std::cout << "Usage: sky [options] file\n\n";
    std::cout << "Available options:\n";
    std::cout << "    --lookup <path>      Sets a lookup path <path> for imports.\n";
    std::cout << "    --timeout <sec>      Kills the running program after <sec> seconds.\n";
}

template<typename T> T& objectField(void* obj, size_t offset) {
	return *(T*)((char*)obj + offset);
}

void String_eq_String(Sky::VM& vm) {
	auto other = vm.pop();
	auto self = vm.pop();

	auto str1 = objectField<const char*>(self.value.object, 0) + 8;
	auto len1 = *objectField<uint64_t*>(self.value.object, 0);

	auto str2 = objectField<const char*>(other.value.object, 0) + 8;
	auto len2 = *objectField<uint64_t*>(other.value.object, 0);

	vm.push(VMValue(len1 == len2 && !strcmp(str1, str2)));
}

VMType* stringType = nullptr;
VMType* u8Type = nullptr;

void String_plus_String(Sky::VM& vm) {
	auto other = vm.pop();
	auto self = vm.pop();

	auto str1 = objectField<const char*>(self.value.object, 0) + 8;
	auto len1 = *objectField<uint64_t*>(self.value.object, 0) - 1;

	auto str2 = objectField<const char*>(other.value.object, 0) + 8;
	auto len2 = *objectField<uint64_t*>(other.value.object, 0) - 1;

	if (!stringType) {
		for (auto& type : vm.chunk.types) {
			if (type->name == "String") {
				stringType = type;
				break;
			}
		}
	}

	if (!u8Type) {
		for (auto& type : vm.chunk.types) {
			if (type->name == "u8[]") {
				u8Type = type;
				break;
			}
		}
	}

	auto newStr = vm.gc.allocObject(stringType);
	auto newArr = vm.gc.allocArray(u8Type, len1 + len2 + 1);

	objectField<void*>(newStr, 0) = newArr;
	auto str = &objectField<char>(newArr, 8);
	memcpy(str, str1, len1);
	memcpy(str + len1, str2, len2);

	vm.push(VMValue(newStr));
}

Scope* makeGlobalScope() {
    auto globals = new Scope(nullptr);

    globals->add("int", &IntType::i64);
    globals->add("u8", &IntType::u8);
    globals->add("u16", &IntType::u16);
    globals->add("u32", &IntType::u32);
    globals->add("u64", &IntType::u64);
    globals->add("i8", &IntType::i8);
    globals->add("i16", &IntType::i16);
    globals->add("i32", &IntType::i32);
    globals->add("i64", &IntType::i64);
    globals->add("bool", &BoolType::instance);
    globals->add("void", &VoidType::instance);
    globals->add("float", &FloatType::f64);
    globals->add("f32", &FloatType::f32);
    globals->add("f64", &FloatType::f64);
    globals->add("null", &NullType::instance);
    globals->add("Ptr", &PointerType::instance);

    std::string fileName;
    if (g_lookupPath.size() && std::ifstream(Sky::g_lookupPath + "/sky/core.sky")) {
        fileName = Sky::g_lookupPath + "/sky/core.sky";
    }
    else if (std::ifstream(Sky::g_homePath + "/.sky/lib/sky/core.sky")) {
        fileName = Sky::g_homePath + "/.sky/lib/sky/core.sky";
    }
    else if (std::ifstream("/usr/local/lib/sky/sky/core.sky")) {
        fileName = "/usr/local/lib/sky/sky/core.sky";
    }

    if (fileName.empty()) {
        error("Unable to locate core library.");
        bail();
    }

    std::ifstream sourceFile(fileName, std::ios::binary);
    Lexer lexer(sourceFile);
    auto source = new SourceFile(fileName, lexer.tokenize());
    if (lexer.hadErrors()) bail();
    Parser parser(*source);
    auto module = parser.parseModDecl();
    if (parser.hadErrors()) bail();

    module->_name = "";
    module->filename = fileName;

    ClassDeclaration::String = module->getClass("String");

    NameResolver resolver(globals);
    resolver.resolve(*module);

    TypeChecker checker;
    checker.check(*module);

    for (auto& cls: module->classes) {
        if (cls->isExported) {
            globals->add(cls->_name, cls);
        }
    }

    for (auto& fun: module->functions) {
        if (fun->isExported) {
            globals->add(fun->name, fun);
        }
    }

	auto eq = ClassDeclaration::String->getMethods("==")[0]->as<FunctionDeclaration>();
	eq->builtin = String_eq_String;

	auto plus = ClassDeclaration::String->getMethods("+")[0]->as<FunctionDeclaration>();
	plus->builtin = String_plus_String;

	return globals;
}

std::string normalizePath(const std::string& path) {
    if (path.empty()) return "./";
    if (path.back() != '/' && path.back() != '\\') {
        return path + "/";
    }
    return path;
}

std::string getImportFile(const std::string& baseFilename, ImportStatement* import) {

    // First looks up file relative to the baseFilename
    // Then looks up in user lib path

    // Base: ~/MyProj/Test.sky
    // import Foo.Bar;
    // 1) ~/MyProj/Foo/Bar.sky
    // 2) ~/MyProj/Foo.sky (import member Bar)
    // 3) <lookup path>/Foo/Bar.sky
    // 4) <lookup path>/Foo.sky (import member Bar)
    // 5) <home path>/.sky/Foo/Bar.sky
    // 6) <home path>/.sky/Foo.sky (import member Bar)
    // 7) /usr/local/lib/sky/Foo/Bar.sky
    // 8) /usr/local/lib/sky/Foo.sky (import member Bar)
    
    std::ifstream file;

    std::string relativeBase("./");
    auto lastSlash = baseFilename.rfind('/');
    if (lastSlash == std::string::npos) lastSlash = baseFilename.rfind('\\');
    if (lastSlash != std::string::npos) {
        relativeBase = baseFilename.substr(0, lastSlash) + "/";
    }

    // local
    std::vector<std::pair<std::string, bool>> tries;
    tries.push_back({relativeBase + import->getFullName("/") + ".sky", true});
    if (!import->all) tries.push_back({relativeBase + import->getBaseName("/") + ".sky", false});

    // additional
    if (g_lookupPath.size()) {
        tries.push_back({g_lookupPath + import->getFullName("/") + ".sky", true});
        if (!import->all) tries.push_back({g_lookupPath + import->getBaseName("/") + ".sky", false});
    }

    // home
    tries.push_back({Sky::g_homePath + ".sky/lib/" + import->getFullName("/") + ".sky", true});
    if (!import->all) tries.push_back({ Sky::g_homePath + ".sky/lib/" + import->getBaseName("/") + ".sky", false});
    
    // global
    tries.push_back({"/usr/local/lib/sky/" + import->getFullName("/") + ".sky", true});
    if (!import->all) tries.push_back({"/usr/local/lib/sky/" + import->getBaseName("/") + ".sky", false});
    
    for (auto&& it: tries) {
        file.open(it.first, std::ios::binary);
        if (file.good()) {
            import->importModule = it.second && !import->all;
            return it.first;
        }
    }

    return "";
}

int main(int argc, char** argv) {

    #ifdef _WIN32
    g_homePath = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH");
    #else
    g_homePath = getenv("HOME");
    #endif
    g_homePath = normalizePath(g_homePath);

    std::string fileName;
    std::vector<std::string> arguments;
    
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--timeout")) {
            g_timeout = std::strtol(argv[++i], nullptr, 10) * 1000;
        } else if (!strcmp(argv[i], "--lookup")) {
            g_lookupPath = argv[++i];
            g_lookupPath = normalizePath(g_lookupPath);
        } else if (!strcmp(argv[i], "--debug")) {
            g_debugPort = std::strtoul(argv[++i], nullptr, 10);
        } else {
            fileName = argv[i++];
            for (int j = i; j < argc; ++j) {
                arguments.push_back(argv[j]);
            }
            break;
        }
    }
    
    if (fileName.empty()) {
        error("Expected file name as last cmd line argument.");
        help();
        return 1;
    }

    try {
        bool errors = false;
        std::ifstream file(fileName, std::ios::binary);
        if (!file.good()) {
            error("File not found: " + fileName);
            return 1;
        }

        ByteCodeChunk chunk;

        // treat as bytecode
        bool isSourcecode = fileName.rfind(".sky") != std::string::npos;

        if (isSourcecode) {
            //std::cout << "Lexing...\n";
            Lexer lexer(file);
            auto source = new SourceFile(fileName, lexer.tokenize());
            if (lexer.hadErrors()) bail();

            //std::cout << "Parsing...\n";
            Parser parser(*source);
            auto module = parser.parseModDecl();
            if (parser.hadErrors()) bail();

            file.close();
            
            module->filename = fileName;

            auto globals = makeGlobalScope();

            std::map<std::string, ModuleDeclaration*> modules {
                { module->getFullName(), module },
            };

            std::vector<ModuleDeclaration*> processImports { module };
            while (!processImports.empty()) {
                auto process = processImports.back();
                processImports.pop_back();

                for (auto&& import: process->imports) {
                    std::string importFilename(getImportFile(process->filename, import));
                    if (importFilename.empty()) {
                        error(import, "Requested import not found: " + import->getFullName());
                        return 1;
                    }

                    auto it = modules.find(importFilename);
                    if (it != modules.end()) {
                        import->module = it->second;
                    }
                    else {
                        std::ifstream importFile(importFilename, std::ios::binary);
                        if (!importFile.good()) {
                            error("Imported file is not readable: " + importFilename);
                            return 1;
                        }

                        Lexer lexer(importFile);
                        auto source = new SourceFile(importFilename, lexer.tokenize());
                        
                        Parser parser(*source);
                        auto importedModule = parser.parseModDecl();
                        if (parser.hadErrors()) bail();
                        importedModule->filename = importFilename;

                        modules.insert(std::make_pair(importFilename, importedModule));
                        import->module = importedModule;

                        if (importedModule) {
                            processImports.push_back(importedModule);
                        }
                    }
                }
            }

            //std::cout << "Resolving names...\n";
            for (auto&& it: modules) {
                NameResolver resolver(globals);
                resolver.resolve(*it.second);
                errors |= resolver.hadErrors();
            }

            if (errors) bail();

            //std::cout << "Resolving generics...\n";
            int numGenerics = 0;
            do {
                numGenerics = 0;
                for (auto&& it: modules) {
                    NameResolver resolver(globals);
                    numGenerics += resolver.resolveGenerics(*it.second);
                    errors |= resolver.hadErrors();
                }
            } while (numGenerics > 0);

            if (errors) bail();

            //std::cout << "Running type checker...\n";
            for (auto&& it: modules) {
                TypeChecker typeChecker;
                typeChecker.check(*it.second);
                errors |= typeChecker.hadErrors();
            }
            
            if (errors) bail();

            //std::cout << "Compiling bytecode...\n";
            ByteCodeCompiler compiler(chunk);
            compiler.compile(*module);
            if (compiler.hadErrors()) bail();
        }

        if (!isSourcecode) {
            std::ifstream inbin(fileName, std::ios::binary);
            inbin >> chunk;
            inbin.close();
        }

		VM vm(chunk, arguments);
        if (g_debugPort > 0) {
            Debugger dbg(g_debugPort, vm);
			return dbg.run();
        }
		else {
			return vm.run();
		}
    }
    catch (const Exception& e) {
        error(e.what());
        return 1;
    }
}