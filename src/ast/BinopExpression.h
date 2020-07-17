// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_BinopExpression_h
#define ast_BinopExpression_h

#include "Expression.h"

namespace Sky {

    class BinopExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::BinopExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        TokenType op;
        Expression* left = nullptr;
        Expression* right = nullptr;
        FunctionDeclaration* function = nullptr;
    };
}

#endif