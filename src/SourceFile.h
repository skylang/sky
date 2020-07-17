// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef SourceFile_h
#define SourceFile_h

#include "ast/Token.h"

#include <string>
#include <vector>

namespace Sky {
    class SourceFile {
    public:
        SourceFile(const std::string& filename, const std::vector<Token>& tokens): filename(filename), tokens(tokens) {}
        std::string getLine(int line) const;
    
    public:
        std::string filename;
        std::vector<Token> tokens;
    };
}

#endif