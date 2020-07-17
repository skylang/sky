// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_NullType_h
#define ast_NullType_h

#include "TypeDeclaration.h"

namespace Sky {
    class NullType: public TypeDeclaration {
    public:
        NullType() { _name = "null"; }
        SKY_GET_TYPE(Sky::NullType, Sky::TypeDeclaration);

    public:
        static NullType instance;
    };
}

#endif