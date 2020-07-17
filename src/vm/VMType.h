// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef vm_VMType_h
#define vm_VMType_h

#include <string>
#include <vector>

namespace Sky {
    class VMType;

    class VMField {
    public:
        std::string name;
        VMType* type;
        size_t offset;
    };

    class VMType {
    public:
        size_t index;
        std::string name;
        bool isObject = false;
        bool isArray = false;
        bool isEnum = false;
        VMType* arrayType = nullptr;
        size_t size = 0;
        size_t alignment = 0;
        size_t objectSize = 0;
        size_t objectAlignment = 0;
        std::vector<VMField> fields;
        std::vector<std::string> enumValues;
        std::vector<VMType*> unionTypes;
    };
}

#endif