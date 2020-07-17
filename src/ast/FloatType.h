// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_FloatType_h
#define ast_FloatType_h

#include "TypeDeclaration.h"

namespace Sky {
    class FloatType: public TypeDeclaration {
    public:
        FloatType(const std::string& name, int bytes): bytes(bytes) { this->_name = name; }
        SKY_GET_TYPE(Sky::FloatType, Sky::TypeDeclaration);

    public:
        int bytes;
        
        static FloatType f64;
        static FloatType f32;
    };
}

#endif