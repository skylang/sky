// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_InterfaceFieldDeclaration_h
#define ast_InterfaceFieldDeclaration_h

#include "Node.h"
#include "InvalidType.h"

#include <string>

namespace Sky {
    class Expression;
    class TypeDeclaration;

    class InterfaceFieldDeclaration: public Node {
    public:
        SKY_GET_TYPE(Sky::InterfaceFieldDeclaration, Sky::Node);
        std::string getDescription();

    public:
        std::string name;
        Expression* typeExpression = nullptr;
        TypeDeclaration* declarationType = &InvalidType::instance;

        int index = 0;
    };
}

#endif