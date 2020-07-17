// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_UnaryExpression_h
#define ast_UnaryExpression_h

#include "Expression.h"

namespace Sky {
    class UnaryExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::UnaryExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        TokenType op;
        Expression* target = nullptr;
    };
}

#endif