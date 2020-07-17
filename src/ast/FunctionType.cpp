// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "FunctionType.h"

#include <sstream>

namespace Sky {
    FunctionType* FunctionType::get(TypeDeclaration* returnType, const std::vector<TypeDeclaration*>& parameterTypes) {
        for (auto&& ftype: funcTypes) {
            if (ftype->returnType == returnType && ftype->parameterTypes == parameterTypes) return ftype;
        }

        std::stringstream sstr;
        sstr << "(";
        for (auto&& p: parameterTypes) {
            sstr << p->getFullName();
            if (&p != &parameterTypes.back()) {
                sstr << ", ";
            }
        }
        sstr << "): " << returnType->getFullName();
        auto ftype = new FunctionType();
        ftype->_name = sstr.str();
        ftype->returnType = returnType;
        ftype->parameterTypes = parameterTypes;
        funcTypes.push_back(ftype);
        return ftype;
    }
}