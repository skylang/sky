// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_GenericReificationExpression_h
#define ast_GenericReificationExpression_h

#include "Expression.h"

#include <vector>

namespace Sky {

    class GenericReificationExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::GenericReificationExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        Expression* baseTypeExpression = nullptr;
        std::vector<Expression*> genericArguments;
    };
}

#endif