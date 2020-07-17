// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ClassDeclaration_h
#define ast_ClassDeclaration_h

#include "TypeDeclaration.h"

#include <string>
#include <vector>

namespace Sky {
    class FunctionDeclaration;
    class FieldDeclaration;
    class GenericParameter;
    
    class ClassDeclaration: public TypeDeclaration {
    public:
        ClassDeclaration() = default;
        ClassDeclaration(const std::string& name) { this->_name = name; }
        SKY_GET_TYPE(Sky::ClassDeclaration, Sky::TypeDeclaration);

        Node* getMember(const std::string& name) override;
        std::vector<Node*> getMethods(const std::string& name) override;

        ClassDeclaration* getReifiedClass(const std::vector<TypeDeclaration*>& typeArgs);

    public:
        bool isResolved = false;
        bool isExported = false;
        std::vector<GenericParameter*> genericParams;
        std::vector<TypeDeclaration*> genericArguments;
        std::vector<FunctionDeclaration*> methods;
        std::vector<FieldDeclaration*> fields;
        static ClassDeclaration* String;
        std::vector<ClassDeclaration*> reifiedClasses;
        ClassDeclaration* genericBase = nullptr;
    };
}

#endif