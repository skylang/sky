// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "InterfaceMethodDeclaration.h"
#include "Parameter.h"
#include "Expression.h"

#include <sstream>

namespace Sky {
    std::string InterfaceMethodDeclaration::getDescription() {
        std::stringstream sstr;
        sstr << "function " << name << "(";
        for (auto& param: parameters) {
            if (param != parameters.front()) {
                sstr << ", ";
            }
            sstr << param->name << ": " << param->declarationType->getFullName();
        }
        sstr << "): " << returnTypeExpression->typeValue->getFullName();
        
        return sstr.str();
    }
}