// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_IdExpression_h
#define ast_IdExpression_h

#include "Expression.h"

#include <string>

namespace Sky {
    class IdExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::IdExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        std::string name;
    };
}

#endif