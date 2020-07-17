// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_IsExpression_h
#define ast_IsExpression_h

#include "Expression.h"

namespace Sky {
    class Implementation;
    
    class IsExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::IsExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        int typeTag;
        Implementation* implementation = nullptr;
        Expression* target = nullptr;
        Expression* typeExpression = nullptr;
    };
}

#endif