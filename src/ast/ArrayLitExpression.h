// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ArrayLitExpression_h
#define ast_ArrayLitExpression_h

#include "Expression.h"
#include "Token.h"

namespace Sky {
    class FunctionDeclaration;
    
    class ArrayLitExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::ArrayLitExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        std::vector<Expression*> elements;
        FunctionDeclaration* constructor = nullptr;
    };
}

#endif