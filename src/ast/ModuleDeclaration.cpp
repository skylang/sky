// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "ModuleDeclaration.h"
#include "FunctionDeclaration.h"
#include "ClassDeclaration.h"
#include "EnumDeclaration.h"
#include "TypeAliasDeclaration.h"

namespace Sky {
    Node* ModuleDeclaration::getMember(const std::string& name) {
        for (auto&& func: functions) {
            if (func->name == name) {
                return func;
            }
        }
        for (auto&& cls: classes) {
            if (cls->_name == name) {
                return cls;
            }
        }
        return nullptr;
    }

    ClassDeclaration* ModuleDeclaration::getClass(const std::string& name) {        
        for (auto&& cls: classes) {
            if (cls->_name == name) {
                return cls;
            }
        }
        return nullptr;
    }

    TypeAliasDeclaration* ModuleDeclaration::getAlias(const std::string& name) {        
        for (auto&& alias: typeAliases) {
            if (alias->_name == name) {
                return alias;
            }
        }
        return nullptr;
    }

    EnumDeclaration* ModuleDeclaration::getEnum(const std::string& name) {
        for (auto&& en: enums) {
            if (en->_name == name) {
                return en;
            }
        }
        return nullptr;
    }

    std::vector<FunctionDeclaration*> ModuleDeclaration::getFunctions(const std::string& name) {
        std::vector<FunctionDeclaration*> funcs;
        for (auto&& function: functions) {
            if (function->name == name) {
                funcs.push_back(function);
            }
        }
        return funcs;
    }

    void ModuleDeclaration::addFunction(FunctionDeclaration* func) {
        functions.push_back(func);
    }

    void ModuleDeclaration::addClass(ClassDeclaration* cls) {
        classes.push_back(cls);
    }
}