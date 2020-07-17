// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_UnionTypeExpression_h
#define ast_UnionTypeExpression_h

#include "Expression.h"

namespace Sky {

    class UnionTypeExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::UnionTypeExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        Expression* base = nullptr;
        Expression* next = nullptr;
    };
}

#endif