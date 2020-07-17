// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_SubscriptExpression_h
#define ast_SubscriptExpression_h

#include "Expression.h"

#include <string>
#include <vector>

namespace Sky {
    class FunctionDeclaration;
    
    class SubscriptExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::SubscriptExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        Expression* callTarget = nullptr;
        std::vector<Expression*> arguments;
        FunctionDeclaration* subscriptFunction = nullptr;
    };
}

#endif