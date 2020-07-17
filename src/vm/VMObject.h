// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef vm_VMObject_h
#define vm_VMObject_h

#include "VMValue.h"

#include <vector>

namespace Sky {
    class VMType;

    struct VMObject {
        bool marked = false;
        const VMType* type;
        char data[];
    };
}

#endif