// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "InterfaceFieldDeclaration.h"
#include "Expression.h"

#include <sstream>

namespace Sky {
    std::string InterfaceFieldDeclaration::getDescription() {
        std::stringstream sstr;
        sstr << "var " << name << ": " << typeExpression->typeValue->getFullName();
        return sstr.str();
    }
}