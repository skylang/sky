// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ast_EnumElement_h
#define ast_EnumElement_h

#include "Node.h"

#include <string>

namespace Sky {    
    class EnumElement: public Node {
    public:
        SKY_GET_TYPE(Sky::EnumElement, Sky::Node);

    public:
        std::string name;
        int index = 0;
    };
}

#endif