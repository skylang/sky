// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef exceptions_h
#define exceptions_h

#include <stdexcept>
#include <string>

namespace Sky {

    class Exception: public std::runtime_error {
    public:
        Exception(const std::string& msg): std::runtime_error(msg) {}
    };

    class LexerException: public Exception {
    public:
        LexerException(const std::string& msg): Exception(msg) {}
    };

    class ParseException: public Exception {
    public:
        ParseException(const std::string& msg): Exception(msg) {}
    };

    class MissingTokenException: public ParseException {
    public:
        MissingTokenException(const std::string& msg): ParseException(msg) {}
    };

    class UnexpectedTokenException: public ParseException {
    public:
        UnexpectedTokenException(const std::string& msg): ParseException(msg) {}
    };

    class DuplicateSymbolException: public Exception {
    public:
        DuplicateSymbolException(const std::string& msg): Exception(msg) {}
    };

    class UnresolvedSymbolException: public Exception {
    public:
        UnresolvedSymbolException(const std::string& msg): Exception(msg) {}
    };

    class TypeException: public Exception {
    public:
        TypeException(const std::string& msg): Exception(msg) {}
    };
}

#endif