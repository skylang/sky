// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_VarDeclaration_h
#define ast_VarDeclaration_h

#include "Statement.h"
#include "InvalidType.h"

#include <string>

namespace Sky {
    class Expression;
    class TypeDeclaration;
    
    class VarDeclaration: public Statement {
    public:
        SKY_GET_TYPE(Sky::VarDeclaration, Sky::Statement);
        SKY_IMPLEMENTATION_STATEMENT_VISITOR;

    public:
        std::string name;
        Expression* typeExpression = nullptr;
        Expression* initializer = nullptr;
        TypeDeclaration* declarationType = &InvalidType::instance;

        int index = 0;
    };
}

#endif