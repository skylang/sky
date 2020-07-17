// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_TypeType_h
#define ast_TypeType_h

#include "TypeDeclaration.h"

namespace Sky {
    class TypeType: public TypeDeclaration {
    public:
        TypeType() { _name = "Type"; }
        SKY_GET_TYPE(Sky::TypeType, Sky::TypeDeclaration);

    public:
        static TypeType instance;
    };
}

#endif