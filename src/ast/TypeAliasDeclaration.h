// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_TypeAliasDeclaration_h
#define ast_TypeAliasDeclaration_h

#include "TypeDeclaration.h"

namespace Sky {
    class Expression;

    class TypeAliasDeclaration: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::TypeAliasDeclaration, Sky::TypeDeclaration);
        
        Node* getMember(const std::string& name) override;
        std::vector<Node*> getMethods(const std::string& name) override;

    public:
        Expression* typeExpression = nullptr;
        bool isExported = false;
    };
}

#endif