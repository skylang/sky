// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_NullableTypeExpression_h
#define ast_NullableTypeExpression_h

#include "Expression.h"

namespace Sky {
    class NullableTypeExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::NullableTypeExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        Expression* baseTypeExpression = nullptr;
    };
}

#endif