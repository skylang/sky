// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef ByteCodeCompiler_h
#define ByteCodeCompiler_h

#include "IStatementVisitor.h"
#include "IExpressionVisitor.h"
#include "Pass.h"

#include <string>
#include <map>
#include <vector>

namespace Sky {
    class ByteCodeChunk;
    class Expression;
    class Node;
    class VMType;
    class TypeDeclaration;
    class FunctionDeclaration;
    class ClassDeclaration;
    class ModuleDeclaration;
    class FieldDeclaration;
    class Parameter;

    class ByteCodeCompiler: public Pass, public IStatementVisitor, public IExpressionVisitor {
    public:
        ByteCodeCompiler(ByteCodeChunk&);
        void compile(ModuleDeclaration&);
        void compile(ClassDeclaration&);
        void compile(FunctionDeclaration&);
        void compile(FieldDeclaration&);
        void compile(Parameter&);

        void visit(BlockStatement&) override;
        void visit(ExpressionStatement&) override;
        void visit(IfStatement&) override;
        void visit(ReturnStatement&) override;
        void visit(VarDeclaration&) override;
        void visit(WhileStatement&) override;

        void visit(ArrayLitExpression&) override;
        void visit(ArrayTypeExpression&) override {}
        void visit(AssignExpression&) override;
        void visit(BinopExpression&) override;
        void visit(CallExpression&) override;
        void visit(CastExpression&) override;
        void visit(GenericReificationExpression&) override {}
        void visit(IdExpression&) override;
        void visit(IsExpression&) override;
        void visit(LitExpression&) override;
        void visit(MapLitExpression&) override;
        void visit(NewExpression&) override;
        void visit(NullableTypeExpression&) override {}
        void visit(PostfixExpression&) override;
        void visit(ScopeExpression&) override;
        void visit(SubscriptExpression&) override;
        void visit(ThisExpression&) override;
        void visit(UnaryExpression&) override;
        void visit(UnionTypeExpression&) override {}

        template<typename T> void visitChildren(T& children) {
            for (auto&& child: children) {
                child->accept(*this);
            }
        }

        template<typename T> void visitChild(T& child) {
            if (child) child->accept(*this);
        }

    private:
        void addFixup(size_t address, FunctionDeclaration* function, bool immediate);
        VMType* mapType(TypeDeclaration* type);

    private:
        struct Fixup {
            size_t address;
            FunctionDeclaration* function;
            bool immediate;
        };
        std::vector<Fixup> functionFixups;
        FunctionDeclaration* function = nullptr;
        std::map<TypeDeclaration*, VMType*> typeMap;

    public:
        ByteCodeChunk& chunk;
        ClassDeclaration* _class = nullptr;
    };
}
#endif