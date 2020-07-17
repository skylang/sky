// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_UnionType_h
#define ast_UnionType_h

#include "TypeDeclaration.h"

#include <set>
#include <vector>

namespace Sky {
    class UnionType: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::UnionType, Sky::TypeDeclaration);

        static UnionType* get(TypeDeclaration* left, TypeDeclaration* right);
        TypeDeclaration* getComplementaryType(TypeDeclaration* t);
        int getTypeTag(TypeDeclaration* t);
        bool containsType(TypeDeclaration* t);

    public:
        std::set<TypeDeclaration*> containedTypes;

        static std::vector<UnionType*> unionTypes;
    };
}

#endif