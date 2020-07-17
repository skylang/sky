// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_InterfaceMethodDeclaration_h
#define ast_InterfaceMethodDeclaration_h

#include "Node.h"

#include <string>
#include <vector>

namespace Sky {
    class BlockStatement;
    class Parameter;
    class Expression;
    class ByteCodeChunk;
    class FunctionType;
    class TypeDeclaration;

    class InterfaceMethodDeclaration: public Node {
    public:
        SKY_GET_TYPE(Sky::InterfaceMethodDeclaration, Sky::Node);
        std::string getDescription();

    public:
        bool isExported = false;
        std::string name;
        std::vector<Parameter*> parameters;
        Expression* returnTypeExpression = nullptr;
        FunctionType* type = nullptr;
        size_t index = 0;
    };
}

#endif