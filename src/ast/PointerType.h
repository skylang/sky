// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_PointerType_h
#define ast_PointerType_h

#include "TypeDeclaration.h"

namespace Sky {
    class PointerType: public TypeDeclaration {
    public:
        PointerType() { _name = "Ptr"; }
        SKY_GET_TYPE(Sky::PointerType, Sky::TypeDeclaration);

    public:        
        static PointerType instance;
    };
}

#endif