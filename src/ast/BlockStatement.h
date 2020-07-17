// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_BlockStatement_h
#define ast_BlockStatement_h

#include "Statement.h"

#include <vector>

namespace Sky {
    class BlockStatement: public Statement {
    public:
        SKY_GET_TYPE(Sky::BlockStatement, Sky::Statement);
        SKY_IMPLEMENTATION_STATEMENT_VISITOR;

    public:
        std::vector<Statement*> statements;
    };
}
#endif