// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_CastExpression_h
#define ast_CastExpression_h

#include "Expression.h"
#include "Token.h"

namespace Sky {
    class Implementation;
    
    class CastExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::CastExpression, Sky::Expression);
        SKY_IMPL_EXPR_VISITOR;

    public:
        Expression* sourceExpression = nullptr;
        Expression* targetTypeExpression = nullptr;
        TypeDeclaration* targetType = nullptr;
        Implementation* implementation = nullptr;
    };
}

#endif