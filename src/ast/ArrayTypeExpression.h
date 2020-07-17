// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ArrayTypeExpression_h
#define ast_ArrayTypeExpression_h

#include "Expression.h"

namespace Sky {

    class ArrayTypeExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::ArrayTypeExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        Expression* baseTypeExpression = nullptr;
    };
}

#endif