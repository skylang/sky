// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ThisExpression_h
#define ast_ThisExpression_h

#include "Expression.h"

namespace Sky {
    class ClassDeclaration;
    
    class ThisExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::ThisExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        ClassDeclaration* _class = nullptr;
    };
}

#endif