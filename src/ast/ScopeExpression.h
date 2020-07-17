// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ScopeExpression_h
#define ast_ScopeExpression_h

#include "Expression.h"

#include <string>

namespace Sky {
    class Symbol;

    class ScopeExpression: public Expression {
    public:
        SKY_GET_TYPE(Sky::ScopeExpression, Sky::Expression);
        SKY_IMPLEMENTATION_EXPRESSION_VISITOR;

    public:
        Expression* scopeTarget = nullptr;
        std::string name;
        Symbol* symbol = nullptr;
    };
}

#endif