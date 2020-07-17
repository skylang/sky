// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "InterfaceDeclaration.h"
#include "InterfaceMethodDeclaration.h"
#include "InterfaceFieldDeclaration.h"

namespace Sky {
    Node* InterfaceDeclaration::getMember(const std::string& name) {
        for (auto&& method: methods) {
            if (method->name == name) {
                return method;
            }
        }
        for (auto&& field: fields) {
            if (field->name == name) {
                return field;
            }
        }
        return nullptr;
    }

    std::vector<Node*> InterfaceDeclaration::getMethods(const std::string& name) {
        std::vector<Node*> result;
        for (auto&& method: methods) {
            if (method->name == name) {
                result.push_back(method);
            }
        }
        return result;
    }
}