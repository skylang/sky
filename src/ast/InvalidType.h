// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_InvalidType_h
#define ast_InvalidType_h

#include "TypeDeclaration.h"

namespace Sky {
    class InvalidType: public TypeDeclaration {
    public:
        InvalidType() { _name = "$invalid"; }
        SKY_GET_TYPE(Sky::InvalidType, Sky::TypeDeclaration);

    public:
        static InvalidType instance;
    };
}

#endif