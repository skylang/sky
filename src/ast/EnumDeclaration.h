// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_EnumDeclaration_h
#define ast_EnumDeclaration_h

#include "TypeDeclaration.h"

#include <vector>

namespace Sky {
    class EnumElement;
    
    class EnumDeclaration: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::EnumDeclaration, Sky::TypeDeclaration);

        Node* getMember(const std::string& name) override;

    public:
        std::vector<EnumElement*> elements;
        bool isExported = false;
    };
}

#endif