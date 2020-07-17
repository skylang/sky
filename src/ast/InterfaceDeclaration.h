// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_InterfaceDeclaration_h
#define ast_InterfaceDeclaration_h

#include "TypeDeclaration.h"

#include <string>
#include <vector>

namespace Sky {
    class InterfaceDeclaration;
    class InterfaceMethodDeclaration;
    class InterfaceFieldDeclaration;
    class ClassDeclaration;
    class FunctionDeclaration;
    class FieldDeclaration;

    class Implementation {
    public:
        InterfaceDeclaration* interface;
        ClassDeclaration* _class;
        std::vector<FunctionDeclaration*> classMethods;
        std::vector<FieldDeclaration*> classFields;
    };
    
    class InterfaceDeclaration: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::InterfaceDeclaration, Sky::TypeDeclaration);

        Node* getMember(const std::string& name) override;
        std::vector<Node*> getMethods(const std::string& name) override;

    public:
        bool isExported = false;
        std::vector<InterfaceMethodDeclaration*> methods;
        std::vector<InterfaceFieldDeclaration*> fields;

        std::map<ClassDeclaration*, Implementation*> implementations;
    };
}

#endif