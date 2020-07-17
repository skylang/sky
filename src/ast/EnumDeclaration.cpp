// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "EnumDeclaration.h"
#include "EnumElement.h"

namespace Sky {
    Node* EnumDeclaration::getMember(const std::string& name) {
        for (auto&& element: elements) {
            if (element->name == name) {
                return element;
            }
        }
        return nullptr;
    }
}