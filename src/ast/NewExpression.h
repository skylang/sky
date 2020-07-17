// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_NewExpression_h
#define ast_NewExpression_h

#include "Expression.h"

#include <string>
#include <vector>

namespace Sky {

    class NewExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::NewExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        Expression* typeExpression = nullptr;
        std::vector<Expression*> arguments;
        FunctionDeclaration* initMethod = nullptr;
    };
}

#endif