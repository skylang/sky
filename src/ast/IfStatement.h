// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_IfStatement_h
#define ast_IfStatement_h

#include "Statement.h"

namespace Sky {
    class Expression;

    class IfStatement: public Statement {
    public:
        SKY_GET_TYPE(Sky::IfStatement, Sky::Statement);
        SKY_IMPLEMENTATION_STATEMENT_VISITOR;

    public:
        Expression* condition = nullptr;
        Statement* trueBranch = nullptr;
        Statement* falseBranch = nullptr;
    };
}

#endif