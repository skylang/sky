// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_GenericParameter_h
#define ast_GenericParameter_h

#include "TypeDeclaration.h"

namespace Sky {
    class GenericParameter: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::GenericParameter, Sky::TypeDeclaration);
    };
}

#endif