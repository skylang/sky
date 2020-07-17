// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_PostfixExpression_h
#define ast_PostfixExpression_h

#include "Expression.h"

namespace Sky {
    class PostfixExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::PostfixExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        TokenType op;
        Expression* target = nullptr;
    };
}

#endif