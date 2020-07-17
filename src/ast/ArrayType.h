// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_ArrayType_h
#define ast_ArrayType_h

#include "TypeDeclaration.h"

namespace Sky {
    class FieldDeclaration;
    
    class ArrayType: public TypeDeclaration {
    public:
        SKY_GET_TYPE(Sky::ArrayType, Sky::TypeDeclaration);

        Node* getMember(const std::string& name) override;

        static ArrayType* get(TypeDeclaration* base);

    public:
        TypeDeclaration* baseType = nullptr;
        FieldDeclaration* lengthField = nullptr;
        static std::map<TypeDeclaration*, ArrayType*> arrayTypes;
    };
}

#endif