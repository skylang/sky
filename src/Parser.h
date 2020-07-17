// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef Parser_h
#define Parser_h

#include "ast/nodes.h"
#include "ast/Token.h"
#include "SourceFile.h"
#include "Pass.h"

#include <vector>
#include <memory>

namespace Sky {
    class Parser: public Pass {
    public:
        Parser(const SourceFile& source, int starttoken = 0): source(source), tokens(source.tokens), token(tokens.begin() + starttoken) {}

        ModuleDeclaration* parseModDecl();
        IdExpression* parseIdExpr(Node* parent);
        FunctionDeclaration* parseFuncDecl(Node* parent, bool external = false);
        Parameter* parseParam(Node* parent);
        VarDeclaration* parseVarDecl(Node* parent);
        BlockStatement* parseBlockStmt(Node* parent);
        ClassDeclaration* parseClassDecl(Node* parent);
        FieldDeclaration* parseFieldDecl(Node* parent);
        EnumDeclaration* parseEnumDecl(Node* parent);
        ImportStatement* parseImportStmt(Node* parent);
        InterfaceDeclaration* parseInterfaceDecl(Node* parent);
        InterfaceMethodDeclaration* parseInterfaceMethodDecl(Node* parent);
        InterfaceFieldDeclaration* parseInterfaceFieldDecl(Node* parent);
        TypeAliasDeclaration* parseTypeAliasDecl(Node* parent);

        Statement* parseStmt(Node* parent);
        ReturnStatement* parseRetStmt(Node* parent);
        ExpressionStatement* parseExprStmt(Node* parent);
        IfStatement* parseIfStmt(Node* parent);
        WhileStatement* parseWhileStmt(Node* parent);

        Expression* parseExpr(Node* parent, int precedence = 0);
        NewExpression* parseNewExpr(Node* parent);
        LitExpression* parseLitExpr(Node* parent);
        CallExpression* parseCallExpr(Node* parent, Expression* callTarget);
        ArrayLitExpression* parseArrayLitExpr(Node* parent);
        MapLitExpression* parseMapLitExpr(Node* parent);
        SubscriptExpression* parseSubscriptExpr(Node* parent, Expression* callTarget);

        Expression* parseTypeExpr(Node* parent);
        GenericReificationExpression* parseGenericReificationExpr(Node* parent, Expression* target);

        bool match(TokenType type);
        bool matchExpr();
        bool matchSecondary();
        bool matchUnary();
        bool matchTypeSecondary();
        bool matchBinary();
        void expect(TokenType type);
        Token eat(TokenType type);
        bool eatOptional(TokenType type);
        Token eat();
        bool eof();
        void expected(TokenType expectedType);
        void expected(const std::string& expected);

    private:
        const std::vector<Token>& tokens;
        const SourceFile& source;
        std::vector<Token>::const_iterator token;

        int numVariables = 0;

        template <typename T> T* addPosition(T* node, const Token& tok) {
            node->line = tok.line;
			node->lineend = (token - 1)->line;
            node->column = tok.column;
            node->source = &source;
            node->firstToken = tok.index;
            return node;
        }

    };
}

#endif