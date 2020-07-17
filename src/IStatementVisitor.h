// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef IStatementVisitor_h
#define IStatementVisitor_h

namespace Sky {
    class IStatementVisitor {
    public:
        virtual void visit(class VarDeclaration&) = 0;
        virtual void visit(class ReturnStatement&) = 0;
        virtual void visit(class BlockStatement&) = 0;
        virtual void visit(class ExpressionStatement&) = 0;
        virtual void visit(class IfStatement&) = 0;
        virtual void visit(class WhileStatement&) = 0;
    };
}

#endif