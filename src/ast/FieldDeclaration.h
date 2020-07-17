// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_FieldDeclaration_h
#define ast_FieldDeclaration_h

#include "Statement.h"
#include "InvalidType.h"

#include <string>

namespace Sky {
    class TypeDeclaration;
    class Expression;
    
    class FieldDeclaration: public Node {
    public:
        SKY_GET_TYPE(Sky::FieldDeclaration, Sky::Node);

    public:
        std::string name;
        Expression* typeExpression = nullptr;
        TypeDeclaration* declarationType = &InvalidType::instance;

        int index = 0;
    };
}

#endif