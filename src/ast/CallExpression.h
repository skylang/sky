// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_CallExpression_h
#define ast_CallExpression_h

#include "Expression.h"

#include <string>
#include <vector>

namespace Sky {
    class CallExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::CallExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        Expression* callTarget = nullptr;
        std::vector<Expression*> arguments;
    };
}

#endif