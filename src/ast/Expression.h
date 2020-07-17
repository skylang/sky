// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_Expression_h
#define ast_Expression_h

#include "Node.h"
#include "../IExpressionVisitor.h"
#include "InvalidType.h"
#include <vector>

#define SKY_IMPL_EXPR_VISITOR void accept(Sky::IExpressionVisitor& v) override { v.visit(*this); }

namespace Sky {
    class TypeDeclaration;
    class FunctionDeclaration;

    class Refinement {
    public:
        Node* node;
        TypeDeclaration* type;
    };

    class Expression: public Node {
    public:
        SKY_GET_TYPE(Sky::Expression, Sky::Node);
        virtual void accept(IExpressionVisitor&) = 0;

    public:
        TypeDeclaration* type = &InvalidType::instance;
        TypeDeclaration* typeValue = &InvalidType::instance;
        Node* node = nullptr;
        Expression* arrayIndex = nullptr;
        Expression* context = nullptr;
        std::vector<FunctionDeclaration*> candidates;
    };
}

#endif