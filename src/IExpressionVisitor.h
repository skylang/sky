// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef IExpressionVisitor_h
#define IExpressionVisitor_h

namespace Sky {
    class IExpressionVisitor {
    public:
        virtual void visit(class IdExpression&) = 0;
        virtual void visit(class LitExpression&) = 0;
        virtual void visit(class CallExpression&) = 0;
        virtual void visit(class BinopExpression&) = 0;
        virtual void visit(class ScopeExpression&) = 0;
        virtual void visit(class NewExpression&) = 0;
        virtual void visit(class AssignExpression&) = 0;
        virtual void visit(class PostfixExpression&) = 0;
        virtual void visit(class UnaryExpression&) = 0;
        virtual void visit(class ThisExpression&) = 0;
        virtual void visit(class CastExpression&) = 0;
        virtual void visit(class IsExpression&) = 0;
        virtual void visit(class ArrayLitExpression&) = 0;
        virtual void visit(class SubscriptExpression&) = 0;
        virtual void visit(class ArrayTypeExpression&) = 0;
        virtual void visit(class MapLitExpression&) = 0;
        virtual void visit(class NullableTypeExpression&) = 0;
        virtual void visit(class UnionTypeExpression&) = 0;
        virtual void visit(class GenericReificationExpression&) = 0;
    };
}

#endif