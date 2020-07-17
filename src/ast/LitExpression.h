// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_LitExpression_h
#define ast_LitExpression_h

#include "Expression.h"
#include "Token.h"

namespace Sky {
    class LitExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::LitExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        Token token;
    };
}

#endif