// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef Lexer_h
#define Lexer_h

#include "ast/Token.h"
#include "Pass.h"

#include <iostream>
#include <vector>

namespace Sky {
    /**
     * Lexer for the sky language
     */
    class Lexer: public Pass {
    public:
        Lexer(std::istream& in): in(in), line(1), column(1) {}
        std::vector<Token> tokenize();

    private:
        int get();
        int peek();
        bool eof();
        bool match(char c);

    private:
        int numtokens = 0;
        std::istream& in;
        int line;
        int column;
        int ch;
    };
}

#endif