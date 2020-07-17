// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_Parameter_h
#define ast_Parameter_h

#include "Node.h"
#include "InvalidType.h"

#include <string>

namespace Sky {
    class Expression;
    class TypeDeclaration;
    
    class Parameter: public Node {
    public:
        SKY_GET_TYPE(Sky::Parameter, Sky::Node);

    public:
        int index = 0;
        std::string name;
        Expression* typeExpression = nullptr;
        TypeDeclaration* declarationType = &InvalidType::instance;
    };
}

#endif