// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_Node_h
#define ast_Node_h

#include "../TypeInfo.h"
#include "Token.h"

#include <iostream>

namespace Sky {
    class SourceFile;
    class Node {
    public:
        Node() {}
        virtual ~Node() {}
        SKY_BASE_TYPE(Sky::Node);

    public:
        Node* parent = nullptr;
        int line = 0;
		int lineend = 0;
		int column = 0;
        const SourceFile* source = nullptr;
        int firstToken = 0;
    };
}
#endif