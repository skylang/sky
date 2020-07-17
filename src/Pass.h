// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef Pass_h
#define Pass_h

#include <string>

namespace Sky {
    class Node;
    struct Token;
    class SourceFile;

    class Pass {
    public:
        bool hadErrors();
        bool hadWarnings();

    protected:
        void info(const std::string& msg);
        void info(const Node& n, const std::string& msg);
        void info(const SourceFile& file, const Token& n, const std::string& msg);

        void warning(const std::string& msg);
        void warning(const Node& n, const std::string& msg);
        void warning(const SourceFile& file, const Token& n, const std::string& msg);

        void error(const std::string& msg);
        void error(const Node& n, const std::string& msg);
        void error(const SourceFile& file, const Token& n, const std::string& msg);

    private:
        bool hasErrors = false;
        bool hasWarnings = false;
    };
}

#endif