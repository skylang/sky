// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ModuleDeclaration_h
#define ast_ModuleDeclaration_h

#include "TypeDeclaration.h"

#include <vector>
#include <string>

namespace Sky {
    class FunctionDeclaration;
    class ClassDeclaration;
    class InterfaceDeclaration;
    class EnumDeclaration;
    class ImportStatement;
    class TypeAliasDeclaration;

    class ModuleDeclaration: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::ModuleDeclaration, Sky::TypeDeclaration);

        Node* getMember(const std::string& name) override;
        ClassDeclaration* getClass(const std::string& name);
        TypeAliasDeclaration* getAlias(const std::string& name);
        EnumDeclaration* getEnum(const std::string& name);
        std::vector<FunctionDeclaration*> getFunctions(const std::string& name);
        void addFunction(FunctionDeclaration* func);
        void addClass(ClassDeclaration* cls);

    public:
        std::vector<ImportStatement*> imports;
        std::vector<FunctionDeclaration*> functions;
        std::vector<ClassDeclaration*> classes;
        std::vector<InterfaceDeclaration*> interfaces;
        std::vector<EnumDeclaration*> enums;
        std::vector<TypeAliasDeclaration*> typeAliases;
        std::string filename;
    };
}

#endif