// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "TypeAliasDeclaration.h"
#include "Expression.h"

namespace Sky {
    Node* TypeAliasDeclaration::getMember(const std::string& name) {
        return typeExpression->typeValue->getMember(name);
    }

    std::vector<Node*> TypeAliasDeclaration::getMethods(const std::string& name) {
        return typeExpression->typeValue->getMethods(name);
    }
}
