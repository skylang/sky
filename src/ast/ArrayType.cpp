// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "ArrayType.h"
#include "FieldDeclaration.h"
#include "IdExpression.h"
#include "IntType.h"

namespace Sky {
    ArrayType* ArrayType::get(TypeDeclaration* base) {
        auto it = arrayTypes.find(base);
        if (it != arrayTypes.end()) return it->second;
        auto atype = new ArrayType();
        atype->_name = base->getFullName() + "[]";
        atype->baseType = base;
        atype->lengthField = new FieldDeclaration();
        atype->lengthField->parent = atype;
        atype->lengthField->name = "length";
        atype->lengthField->typeExpression = new IdExpression();
        static_cast<IdExpression*>(atype->lengthField->typeExpression)->name = "u64";
        atype->lengthField->declarationType = atype->lengthField->typeExpression->type = &IntType::u64;
        atype->lengthField->index = 0;
        arrayTypes.insert(std::make_pair(base, atype));
        return atype;
    }

    Node* ArrayType::getMember(const std::string& name) {
        return name == "length" ? lengthField : nullptr;
    }
}