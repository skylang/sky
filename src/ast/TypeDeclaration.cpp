// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "TypeDeclaration.h"
#include "ModuleDeclaration.h"

namespace Sky {
    std::string TypeDeclaration::getFullName() {
        ModuleDeclaration* mod = parent ? parent->as<ModuleDeclaration>() : nullptr;
        if (mod && !mod->_name.empty()) {
            return mod->getFullName() + "." + _name;
        }
        return _name;
    }
}