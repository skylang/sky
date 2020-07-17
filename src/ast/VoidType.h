// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_VoidType_h
#define ast_VoidType_h

#include "TypeDeclaration.h"

namespace Sky {
    class VoidType: public TypeDeclaration {
    public:
        VoidType() { _name = "void"; }
        SKY_GET_TYPE(Sky::VoidType, Sky::TypeDeclaration);

    public:
        static VoidType instance;
    };
}

#endif