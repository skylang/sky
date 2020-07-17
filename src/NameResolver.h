// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef NameResolver_h
#define NameResolver_h

#include "IStatementVisitor.h"
#include "IExpressionVisitor.h"
#include "Pass.h"

#include <string>

namespace Sky {
    class Scope;
    class Node;
    class ModuleDeclaration;
    class ClassDeclaration;
    class FunctionDeclaration;
    class Parameter;
    class FieldDeclaration;
    class ImportStatement;
    class InterfaceDeclaration;
    class InterfaceMethodDeclaration;
    class InterfaceFieldDeclaration;
    class TypeAliasDeclaration;

    class NameResolver: public Pass, public IStatementVisitor, public IExpressionVisitor {
    public:
        NameResolver(Scope* globals);
        void resolve(ModuleDeclaration&);
        void resolve(ClassDeclaration&);
        void resolve(FunctionDeclaration&);
        void resolve(Parameter&);
        void resolve(ImportStatement&);
        void resolve(InterfaceDeclaration&);
        void resolve(InterfaceMethodDeclaration&);
        void resolve(InterfaceFieldDeclaration&);
        void resolve(TypeAliasDeclaration&);
        void resolve(FieldDeclaration&);

        void visit(BlockStatement&) override;
        void visit(ExpressionStatement&) override;
        void visit(IfStatement&) override;
        void visit(ReturnStatement&) override;
        void visit(VarDeclaration&) override;
        void visit(WhileStatement&) override;

        void visit(ArrayLitExpression&) override;
        void visit(ArrayTypeExpression&) override;
        void visit(AssignExpression&) override;
        void visit(BinopExpression&) override;
        void visit(CallExpression&) override;
        void visit(CastExpression&) override;
        void visit(GenericReificationExpression&) override;
        void visit(IdExpression&) override;
        void visit(IsExpression&) override;
        void visit(LitExpression&) override {}
        void visit(MapLitExpression&) override;
        void visit(NewExpression&) override;
        void visit(NullableTypeExpression&) override;
        void visit(PostfixExpression&) override;
        void visit(ScopeExpression&) override;
        void visit(SubscriptExpression&) override;
        void visit(ThisExpression&) override {};
        void visit(UnaryExpression&) override;
        void visit(UnionTypeExpression&) override;

        int resolveGenerics(ModuleDeclaration&);

        template<typename T> void visitChildren(T& children) {
            for (auto&& child: children) {
                visitChild(child);
            }
        }

        template<typename T> void visitChild(T& child) {
            if (child) {
                child->accept(*this);
            }
        }
        
    private:
        Scope* scope;
    };
}
#endif