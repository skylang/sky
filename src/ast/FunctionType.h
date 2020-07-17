// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_FunctionType_h
#define ast_FunctionType_h

#include "TypeDeclaration.h"

#include <vector>

namespace Sky {
    class FunctionType: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::FunctionType, Sky::TypeDeclaration);

        static FunctionType* get(TypeDeclaration* returnType, const std::vector<TypeDeclaration*>& parameterTypes);

    public:
        std::vector<TypeDeclaration*> parameterTypes;
        TypeDeclaration* returnType = nullptr;

        static std::vector<FunctionType*> funcTypes;
    };
}

#endif