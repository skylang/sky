// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_Statement_h
#define ast_Statement_h

#include "Node.h"
#include "../IStatementVisitor.h"

#include <string>

#define SKY_IMPLEMENTATION_STATEMENT_VISITOR void accept(Sky::IStatementVisitor& v) override { v.visit(*this); }

namespace Sky {
    class Statement: public Node {
    public:
        SKY_GET_TYPE(Sky::Statement, Sky::Node);
        virtual void accept(IStatementVisitor&) = 0;

    public:
        bool returns = false;
    };
}

#endif