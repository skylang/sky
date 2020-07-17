// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_TypeDeclaration_h
#define ast_TypeDeclaration_h

#include "Node.h"

#include <string>
#include <vector>

namespace Sky {
    class TypeDeclaration: public Node {
    public:
        SKY_GET_TYPE(Sky::TypeDeclaration, Sky::Node);
        virtual Node* getMember(const std::string& name) { return nullptr; }
        virtual std::vector<Node*> getMethods(const std::string& name) { return {}; }
        std::string getFullName();

    public:
        std::string _name;
    };
}

#endif