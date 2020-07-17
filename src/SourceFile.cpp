// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "SourceFile.h"
#include <sstream>

namespace Sky {
    std::string SourceFile::getLine(int line) const {
        std::stringstream sstr;
        for (auto& tok: tokens) {
            if (tok.line > line) break;
            if (tok.line == line) sstr << tok.trivia << tok.value;
        }
        return sstr.str();
    }
}