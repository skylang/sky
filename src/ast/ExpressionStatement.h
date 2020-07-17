// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ExpressionStatement_h
#define ast_ExpressionStatement_h

#include "Statement.h"

namespace Sky {
    class Expression;

    class ExpressionStatement: public Statement {
    public:
        SKY_GET_TYPE(Sky::ExpressionStatement, Sky::Statement);
        SKY_IMPLEMENTATION_STATEMENT_VISITOR;

    public:
        Expression* expression = nullptr;
    };
}

#endif