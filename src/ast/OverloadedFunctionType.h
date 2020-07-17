// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_OverloadedFunctionType_h
#define ast_OverloadedFunctionType_h

#include "TypeDeclaration.h"

namespace Sky {
    class OverloadedFunctionType: public TypeDeclaration {
    public:
        OverloadedFunctionType() { _name = "$overloaded"; }
        SKY_GET_TYPE(Sky::OverloadedFunctionType, Sky::TypeDeclaration);

    public:
        static OverloadedFunctionType instance;
    };
}

#endif