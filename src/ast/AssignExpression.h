// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_AssignExpression_h
#define ast_AssignExpression_h

#include "BinopExpression.h"

namespace Sky {
    class AssignExpression: public BinopExpression {
    public:
        SKY_GET_TYPE(Sky::AssignExpression, Sky::BinopExpression);
        SKY_IMPL_EXPR_VISITOR;
    };
}

#endif