// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_MapLitExpression_h
#define ast_MapLitExpression_h

#include "Expression.h"
#include "Token.h"

namespace Sky {
    class FunctionDeclaration;
    class TypeDeclaration;
    
    class MapLitExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::MapLitExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        std::vector<Expression*> keys;
        std::vector<Expression*> values;
        FunctionDeclaration* constructor = nullptr;
        TypeDeclaration* keyArrayType = &InvalidType::instance;
        TypeDeclaration* valueArrayType = &InvalidType::instance;
    };
}

#endif