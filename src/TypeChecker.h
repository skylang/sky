// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef TypeChecker_h
#define TypeChecker_h

#include "IStatementVisitor.h"
#include "IExpressionVisitor.h"
#include "Pass.h"
#include "ast/TypeDeclaration.h"
#include "ast/InvalidType.h"

#include <string>
#include <vector>
#include <map>

namespace Sky {
    class Refinement;
    class TypeDeclaration;
    class Node;
    class Statement;
    class Expression;
    class Parameter;
    class FunctionDeclaration;
    class ClassDeclaration;
    class ModuleDeclaration;
    class InterfaceDeclaration;

    class TypeChecker: public Pass, public IStatementVisitor, public IExpressionVisitor {
    public:
        TypeChecker();
        void check(ModuleDeclaration&);
        void check(ClassDeclaration&);
        void check(FunctionDeclaration&);

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

        template<typename T> bool unify(T&& child, TypeDeclaration* with) {
            auto oldExpectedType = expectedType;
            expectedType = with;
            bool success = true;
            if (child) {
                child->accept(*this);
                if (expectedType && !isAssignableFrom(expectedType, getType(child))) {
                    if (child->type != &InvalidType::instance) {
                        error(*child, child->type->getFullName() + " is not coercible to " + expectedType->getFullName());
                        success = false;
                    }
                }
                else if (expectedType) {
                    child = addCast(child, expectedType);
                }
            }
            expectedType = oldExpectedType;
            return success;
        }

        template<typename T> bool unifyChildren(T& children, TypeDeclaration* with) {
            auto oldExpectedType = expectedType;
            expectedType = with;
            bool success = true;
            for (auto&& child: children) {
                if (child) {
                    child->accept(*this);
                    if (expectedType && !isAssignableFrom(expectedType, getType(child))) {
                        if (child->type != &InvalidType::instance) {
                            error(*child, child->type->getFullName() + " is not coercible to " + expectedType->getFullName());
                            success = false;
                        }
                    }
                    else if (expectedType) {
                        child = addCast(child, expectedType);
                    }
                }
            }
            expectedType = oldExpectedType;
            return success;
        }

        void negateRefinements();
        
        std::vector<TypeDeclaration*> getTypes(const std::vector<Expression*>& arguments);
        TypeDeclaration* getType(Expression* expr);
        TypeDeclaration* getType(VarDeclaration* var);
        TypeDeclaration* getType(Parameter* param);

    private:
        std::vector<FunctionDeclaration*> findOverload(Expression* target, const std::vector<TypeDeclaration*>& argtypes);
        std::vector<FunctionDeclaration*> findOverload(const std::vector<FunctionDeclaration*>& funcs, const std::vector<Expression*>& args);
        std::vector<FunctionDeclaration*> findOverload(const std::vector<FunctionDeclaration*>& funcs, const std::vector<TypeDeclaration*>& argTypes);

        void printMissingMembers(ClassDeclaration* cls, InterfaceDeclaration* iface);

        FunctionDeclaration* function = nullptr;
        BlockStatement* block = nullptr;
        ClassDeclaration* _class = nullptr;
        TypeDeclaration* expectedType = nullptr;

        bool returns = false;

        std::vector<std::map<Node*, std::vector<Refinement>>> refinements;
    };
}
#endif