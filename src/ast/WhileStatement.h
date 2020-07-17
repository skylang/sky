// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_WhileStatement_h
#define ast_WhileStatement_h

#include "Statement.h"

namespace Sky {
    class Expression;

    class WhileStatement: public Statement {
    public:
        SKY_GET_TYPE(Sky::WhileStatement, Sky::Statement);
        SKY_IMPLEMENTATION_STATEMENT_VISITOR;

    public:
        Expression* condition = nullptr;
        Statement* body = nullptr;
    };
}

#endif