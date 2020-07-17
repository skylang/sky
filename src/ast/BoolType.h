// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_BoolType_h
#define ast_BoolType_h

#include "TypeDeclaration.h"

namespace Sky {
    class BoolType: public TypeDeclaration {
    public:
        BoolType() { _name = "bool"; }
        SKY_GET_TYPE(Sky::BoolType, Sky::TypeDeclaration);

    public:
        static BoolType instance;
    };
}

#endif