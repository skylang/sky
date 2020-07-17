// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_FunctionDeclaration_h
#define ast_FunctionDeclaration_h

#include "Node.h"

#include <string>
#include <vector>

namespace Sky {
    class Parameter;
    class Expression;
    class FunctionType;
    class Statement;
    class VM;

    typedef void(*BuiltinFunction)(VM&);

    class FunctionDeclaration: public Node {
    public:
        SKY_GET_TYPE(Sky::FunctionDeclaration, Sky::Node);

    public:
        bool returns = false;
        bool isExported = false;
        std::string name;
        std::vector<Parameter*> parameters;
        Expression* returnTypeExpression = nullptr;
        FunctionType* declarationType = nullptr;
        std::vector<Statement*> statements;
        size_t opcodeStart = 0xdeadbeef;
        int numVariables = 0;
        bool isPrototype = false;
        bool isExternal = false;
        BuiltinFunction builtin = nullptr;
    };
}

#endif