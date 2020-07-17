// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "Parser.h"
#include "ast/nodes.h"
#include "exceptions.h"

#include <memory>

namespace Sky {
    std::map<TokenType, int> unaryPrecedenceMap {
        {TokenType::PlusPlus, 13},
        {TokenType::MinusMinus, 13},

        {TokenType::Minus, 12},
        {TokenType::Tilde, 12},
        {TokenType::ExclamationMark, 12},
    };

    std::map<TokenType, int> precedenceMap {
        {TokenType::Is, 1},

        // Assignment
        {TokenType::Equals, 1},
        {TokenType::PlusEquals, 1},
        {TokenType::MinusEquals, 1},
        {TokenType::AsteriskEquals, 1},
        {TokenType::SlashEquals, 1},
        {TokenType::PercentEquals, 1},
        {TokenType::AmpEquals, 1},
        {TokenType::PipeEquals, 1},
        {TokenType::CaretEquals, 1},

        // logical
        {TokenType::PipePipe, 2},
        {TokenType::AmpAmp, 3},

        // binary
        {TokenType::Pipe, 4},
        {TokenType::Caret, 5},
        {TokenType::Amp, 6},

        // equality
        {TokenType::EqualsEquals, 7},
        {TokenType::ExclamationMarkEquals, 7},

        // comparison
        {TokenType::LessThan, 8},
        {TokenType::GreaterThan, 8},
        {TokenType::LessThanEquals, 8},
        {TokenType::GreaterThanEquals, 8},

        // bit shift

        // addition
        {TokenType::Plus, 10},
        {TokenType::Minus, 10},

        // multiplication
        {TokenType::Asterisk, 11},
        {TokenType::Slash, 11},
        {TokenType::Percent, 11},

        {TokenType::ExclamationMark, 12},
        {TokenType::Tilde, 12},

        // cast
        {TokenType::As, 13},

        // access
        {TokenType::Period, 14},
        {TokenType::BracketOpen, 14},
        {TokenType::ParenOpen, 14},
        {TokenType::PlusPlus, 14},
        {TokenType::MinusMinus, 14},
    };

    ModuleDeclaration* Parser::parseModDecl() {
        auto moddecl = new ModuleDeclaration();
        auto startToken = eat(TokenType::Module);
        moddecl->line = startToken.line;
        moddecl->column = startToken.column;

        auto name = eat(TokenType::Identifier).value;
        while (eatOptional(TokenType::Period)) {
            name += ".";
            name += eat(TokenType::Identifier).value;
        }
        moddecl->_name = name;

        eat(TokenType::CurlyOpen);

        bool exportNext = false;
        bool externalNext = false;

        while (!eof() && !match(TokenType::CurlyClose)) {
            if (eatOptional(TokenType::Export)) {
                exportNext = true;
                continue;
            }

            if (eatOptional(TokenType::External)) {
                externalNext = true;
                continue;
            }

            if (match(TokenType::Function)) {
                auto fun = parseFuncDecl(moddecl, externalNext);
                if (exportNext) fun->isExported = true;
                moddecl->functions.push_back(fun);
            }
            else if (match(TokenType::Class)) {
                auto cls = parseClassDecl(moddecl);
                if (exportNext) cls->isExported = true;
                moddecl->classes.push_back(cls);
            }
            else if (match(TokenType::Interface)) {
                auto iface = parseInterfaceDecl(moddecl);
                if (exportNext) iface->isExported = true;
                moddecl->interfaces.push_back(iface);
            }
            else if (match(TokenType::Enum)) {
                auto en = parseEnumDecl(moddecl);
                if (exportNext) en->isExported = true;
                moddecl->enums.push_back(en);
            }
            else if (match(TokenType::Type)) {
                auto al = parseTypeAliasDecl(moddecl);
                if (exportNext) al->isExported = true;
                moddecl->typeAliases.push_back(al);
            }
            else if (match(TokenType::Import)) {
                auto imp = parseImportStmt(moddecl);
                moddecl->imports.push_back(imp);
            }
            else {
                eat();
                expected("module member declaration");
            }

            exportNext = false;
            externalNext = false;
        }
        auto endToken = eat(TokenType::CurlyClose);
        moddecl->lineend = endToken.line;

        return moddecl;
    }

    ImportStatement* Parser::parseImportStmt(Node* parent) {
        auto import = new ImportStatement();
        import->parent = parent;
        auto st = eat(TokenType::Import);

        import->parts.push_back(eat(TokenType::Identifier).value);
        while (eatOptional(TokenType::Period)) {
            if (eatOptional(TokenType::Asterisk)) {
                import->all = true;
                break;
            }
            else {
                import->parts.push_back(eat(TokenType::Identifier).value);
            }
        }
        eat(TokenType::Semicolon);
        return addPosition(import, st);
    }

    FunctionDeclaration* Parser::parseFuncDecl(Node* parent, bool external) {
        auto fun = new FunctionDeclaration();
        fun->parent = parent;
        numVariables = 0;
        auto startToken = eat(TokenType::Function);
        if (match(TokenType::Identifier)) {
            fun->name = eat().value;
        }
        else if (match(TokenType::BracketOpen)) {
            eat();
            eat(TokenType::BracketClose);
            fun->name = "[]";
        }
        else if (matchBinary()) {
            fun->name = eat().value;
        }
        else {
            expected("function name");
        }

        eat(TokenType::ParenOpen);
        while (!eof() && !match(TokenType::ParenClose)) {
            auto param = parseParam(fun);
            param->index = fun->parameters.size();
            fun->parameters.push_back(param);
            if (match(TokenType::Comma)) {
                eat();
            }
            else {
                break;
            }
        }
        eat(TokenType::ParenClose);
        
        if (eatOptional(TokenType::Colon)) {
            fun->returnTypeExpression = parseTypeExpr(fun);
        }

        if (external) {
            eat(TokenType::Semicolon);
        }
        else {
            eat(TokenType::CurlyOpen);
            while (!eof() && !match(TokenType::CurlyClose)) {
                if (match(TokenType::Semicolon)) {
                    eat();
                }
                else {
                    fun->statements.push_back(parseStmt(fun));
                }
            }
            eat(TokenType::CurlyClose);
        }

        fun->numVariables = numVariables;
        fun->isExternal = external;
        return addPosition(fun, startToken);
    }

    BlockStatement* Parser::parseBlockStmt(Node* parent) {
        auto block = new BlockStatement();
        block->parent = parent;
        auto startToken = eat(TokenType::CurlyOpen);

        while (!eof() && !match(TokenType::CurlyClose)) {
            if (match(TokenType::Semicolon)) {
                eat();
            }
            else {
                block->statements.push_back(parseStmt(block));
            }
        }
        eat(TokenType::CurlyClose);
        return addPosition(block, startToken);
    }

    Statement* Parser::parseStmt(Node* parent) {
        if (match(TokenType::Return)) {
            return parseRetStmt(parent);
        }
        else if (match(TokenType::CurlyOpen)) {
            return parseBlockStmt(parent);
        }
        else if (match(TokenType::If)) {
            return parseIfStmt(parent);
        }
        else if (match(TokenType::While)) {
            return parseWhileStmt(parent);
        }
        else if (match(TokenType::Var)) {
            return parseVarDecl(parent);
        }
        else if (matchExpr()) {
            return parseExprStmt(parent);
        }
        else {
            eat();
            expected("statement");
            return nullptr; // TODO: return null object
        }
    }

    ExpressionStatement* Parser::parseExprStmt(Node* parent) {
        auto stmt = new ExpressionStatement();
        stmt->parent = parent;
        auto startToken = *token;
        stmt->expression = parseExpr(stmt);
        eat(TokenType::Semicolon);
        return addPosition(stmt, startToken);
    }

    ReturnStatement* Parser::parseRetStmt(Node* parent) {
        auto ret = new ReturnStatement();
        ret->parent = parent;
        auto startToken = eat(TokenType::Return);
        if (matchExpr()) {
            ret->expression = parseExpr(ret);
        }
        eat(TokenType::Semicolon);
        return addPosition(ret, startToken);
    }

    IfStatement* Parser::parseIfStmt(Node* parent) {
        auto ifStmt = new IfStatement();
        ifStmt->parent = parent;

        auto startToken = eat(TokenType::If);
        eat(TokenType::ParenOpen);
        ifStmt->condition = parseExpr(ifStmt);
        eat(TokenType::ParenClose);
        ifStmt->trueBranch = parseStmt(ifStmt);
        if (eatOptional(TokenType::Else)) {
            ifStmt->falseBranch = parseStmt(ifStmt);
        }
        return addPosition(ifStmt, startToken);
    }

    WhileStatement* Parser::parseWhileStmt(Node* parent) {
        auto whileStmt = new WhileStatement();
        whileStmt->parent = parent;

        auto startToken = eat(TokenType::While);
        eat(TokenType::ParenOpen);
        whileStmt->condition = parseExpr(whileStmt);
        eat(TokenType::ParenClose);
        whileStmt->body = parseStmt(whileStmt);
        return addPosition(whileStmt, startToken);
    }

    Expression* Parser::parseTypeExpr(Node* parent) {
        Expression* expression = nullptr;
        if (match(TokenType::Identifier) || match(TokenType::Null)) {
            auto start = eat();
            auto id = new IdExpression();
            id->parent = parent;
            id->name = start.value;
            expression = addPosition(id, start);
        }
        else {
            expected("Type name");
        }

        while (matchTypeSecondary()) {
            if (match(TokenType::BracketOpen)) {
                auto st = eat();
                eat(TokenType::BracketClose);
                auto arr = new ArrayTypeExpression();
                arr->parent = parent;
                arr->baseTypeExpression = expression;
                arr->baseTypeExpression->parent = arr;
                expression = addPosition(arr, st);
            }
            else if (match(TokenType::LessThan)) {
                expression = parseGenericReificationExpr(parent, expression);
            }
            else if (match(TokenType::Pipe)) {
                auto st = eat();
                auto un = new UnionTypeExpression();
                un->parent = parent;
                un->base = expression;
                un->base->parent = un;
                un->next = parseTypeExpr(un);
                expression = addPosition(un, st);
            }
            else if (match(TokenType::QuestionMark)) {
                auto st = eat();
                auto nullable = new NullableTypeExpression();
                nullable->parent = parent;
                nullable->baseTypeExpression = expression;
                nullable->baseTypeExpression->parent = nullable;
                expression = addPosition(nullable, st);
            }
            else if (match(TokenType::Period)) {
                auto st = eat();
                auto name = eat(TokenType::Identifier);
                auto scope = new ScopeExpression();
                scope->parent = parent;
                scope->name = name.value;
                scope->scopeTarget = expression;
                scope->scopeTarget->parent = scope;
                expression = addPosition(scope, st);
            }
            else {
                eat();
                expected("secondary type expression.");
            }
        }

        return expression;
    }

    GenericReificationExpression* Parser::parseGenericReificationExpr(Node* parent, Expression* target) {
        auto startToken = eat(TokenType::LessThan);
        auto rei = new GenericReificationExpression();
        rei->parent = parent;
        rei->baseTypeExpression = target;
        rei->baseTypeExpression->parent = rei;
        while (!eof() && !match(TokenType::GreaterThan)) {
            rei->genericArguments.push_back(parseTypeExpr(rei));
            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::GreaterThan);
        return addPosition(rei, startToken);
    }
    
    Expression* Parser::parseExpr(Node* parent, int precedence) {
        Expression* expression = nullptr;
        if (match(TokenType::Integer) || match(TokenType::Float) || match(TokenType::String) || match(TokenType::Boolean) || match(TokenType::Null)) {
            expression = parseLitExpr(parent);
        }
        else if (match(TokenType::Identifier)) {
            expression = parseIdExpr(parent);
        }
        else if (matchUnary()) {
            auto st = eat();
            auto un = new UnaryExpression();
            un->parent = parent;
            un->op = st.type;
            un->target = parseExpr(un, unaryPrecedenceMap[st.type]);
            expression = addPosition(un, st);
        }
        else if (match(TokenType::ParenOpen)) {
            eat();
            expression = parseExpr(parent);
            eat(TokenType::ParenClose);
        }
        else if (match(TokenType::New)) {
            expression = parseNewExpr(parent);
        }
        else if (match(TokenType::This)) {
            auto st = eat();
            auto thisExpr = new ThisExpression();
            thisExpr->parent = parent;
            expression = addPosition(thisExpr, st);
        }
        else if (match(TokenType::BracketOpen)) {
            expression = parseArrayLitExpr(parent);
        }
        else if (match(TokenType::CurlyOpen)) {
            expression = parseMapLitExpr(parent);
        }
        else {
            expected("primary expression or prefix operator");
            return nullptr; // TODO return null object
        }

        while (matchSecondary()) {
            auto it = precedenceMap.find(token->type);
            if (it == precedenceMap.end()) {
                error(source, *token, "No precedence for operator '" + token->value + "'");
                eat();
                continue;
            }
            if (it->second <= precedence) {
                break;
            }
            int myprec = it->second;

            if (match(TokenType::ParenOpen)) {
                expression = parseCallExpr(parent, expression);
            }
            else if (match(TokenType::BracketOpen)) {
                expression = parseSubscriptExpr(parent, expression);
            }
            else if (match(TokenType::Period)) {
                auto st = eat();
                auto scope = new ScopeExpression();
                scope->parent = parent;
                scope->name = eat(TokenType::Identifier).value;
                scope->scopeTarget = expression;
                scope->scopeTarget->parent = scope;
                expression = addPosition(scope, st);
            }
            else if (match(TokenType::MinusMinus) || match(TokenType::PlusPlus)) {
                auto st = eat();
                auto post = new PostfixExpression();
                post->parent = parent;
                post->op = st.type;
                post->target = expression;
                post->target->parent = post;
                expression = addPosition(post, st);
            }
            else if (match(TokenType::Is)) {
                auto startToken = eat(TokenType::Is);
                auto is = new IsExpression();
                is->parent = parent;
                is->typeExpression = parseTypeExpr(is);
                is->target = expression;
                is->target->parent = is;
                expression = addPosition(is, startToken);
            }
            else if (match(TokenType::As)) {
                auto startToken = eat();
                auto cast = new CastExpression();
                cast->parent = parent;
                cast->sourceExpression = expression;
                cast->sourceExpression->parent = cast;
                cast->targetTypeExpression = parseTypeExpr(cast);
                expression = addPosition(cast, startToken);
            }
            else if (matchBinary()) {
                auto op = eat();
                if (
                    op.type == TokenType::Equals ||
                    op.type == TokenType::PlusEquals ||
                    op.type == TokenType::MinusEquals ||
                    op.type == TokenType::AsteriskEquals ||
                    op.type == TokenType::SlashEquals
                ) {
                    auto assign = new AssignExpression();
                    assign->parent = parent;
                    assign->op = op.type;
                    assign->left = expression;
                    assign->left->parent = assign;
                    assign->right = parseExpr(assign, myprec);
                    expression = addPosition(assign, op);
                }
                else {
                    auto binop = new BinopExpression();
                    binop->parent = parent;
                    binop->op = op.type;
                    binop->left = expression;
                    binop->left->parent = binop;
                    binop->right = parseExpr(binop, myprec);
                    expression = addPosition(binop, op);
                }
            }
            else {
                eat();
                expected("secondary expression");
                break;
            }
        }
        return expression;
    }

    NewExpression* Parser::parseNewExpr(Node* parent) {
        auto newExpr = new NewExpression();
        newExpr->parent = parent;

        auto startToken = eat(TokenType::New);
        newExpr->typeExpression = parseTypeExpr(newExpr);
        
        if (eatOptional(TokenType::ParenOpen)) {
            while (!eof() && !match(TokenType::ParenClose)) {
                newExpr->arguments.push_back(parseExpr(newExpr));
                if (!eatOptional(TokenType::Comma)) {
                    break;
                }
            }
            eat(TokenType::ParenClose);
        }
        return addPosition(newExpr, startToken);
    }

    CallExpression* Parser::parseCallExpr(Node* parent, Expression* callTarget) {
        auto call = new CallExpression();
        call->parent = parent;
        call->callTarget = callTarget;

        auto startToken = eat(TokenType::ParenOpen);
        while (!eof() && !match(TokenType::ParenClose)) {
            call->arguments.push_back(parseExpr(call));
            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::ParenClose);
        return addPosition(call, startToken);
    }

    SubscriptExpression* Parser::parseSubscriptExpr(Node* parent, Expression* callTarget) {
        auto sub = new SubscriptExpression();
        sub->parent = parent;
        sub->callTarget = callTarget;

        auto startToken = eat(TokenType::BracketOpen);
        while (!eof() && !match(TokenType::BracketClose)) {
            sub->arguments.push_back(parseExpr(sub));
            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::BracketClose);
        return addPosition(sub, startToken);
    }

    LitExpression* Parser::parseLitExpr(Node* parent) {
        if (!(match(TokenType::Integer) || match(TokenType::Float) || match(TokenType::String) || match(TokenType::Boolean) || match(TokenType::Null))) {
            expected("literal");
        }
        auto lit = new LitExpression();
        lit->parent = parent;
        auto tok = eat();
        lit->token = tok;
        return addPosition(lit, tok);
    }

    IdExpression* Parser::parseIdExpr(Node* parent) {
        auto id = new IdExpression();
        id->parent = parent;
        auto startToken = eat(TokenType::Identifier);
        id->name = startToken.value;
        return addPosition(id, startToken);
    }

    Parameter* Parser::parseParam(Node* parent) {
        auto param = new Parameter();
        param->parent = parent;

        auto startToken = eat(TokenType::Identifier);
        param->name = startToken.value;
        eat(TokenType::Colon);
        param->typeExpression = parseTypeExpr(param);
        return addPosition(param, startToken);
    }

    VarDeclaration* Parser::parseVarDecl(Node* parent) {
        auto var = new VarDeclaration();
        var->parent = parent;

        auto startToken = eat(TokenType::Var);
        var->name = eat(TokenType::Identifier).value;

        if (eatOptional(TokenType::Colon)) {
            var->typeExpression = parseTypeExpr(var);
        }

        if (eatOptional(TokenType::Equals)) {
            var->initializer = parseExpr(var);
        }

        if (!var->typeExpression && !var->initializer) {
            expected("type or initializer expression.");
        }

        eat(TokenType::Semicolon);
        auto vardecl = addPosition(var, startToken);
        vardecl->index = numVariables++;
        return vardecl;
    }

    FieldDeclaration* Parser::parseFieldDecl(Node* parent) {
        auto field = new FieldDeclaration();
        field->parent = parent;

        auto startToken = eat(TokenType::Var);
        field->name = eat(TokenType::Identifier).value;
        eat(TokenType::Colon);
        field->typeExpression = parseTypeExpr(field);
        eat(TokenType::Semicolon);
        return addPosition(field, startToken);
    }

    ArrayLitExpression* Parser::parseArrayLitExpr(Node* parent) {
        auto arr = new ArrayLitExpression();
        arr->parent = parent;

        auto startToken = eat(TokenType::BracketOpen);
        while (!eof() && !match(TokenType::BracketClose)) {
            arr->elements.push_back(parseExpr(arr));
            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::BracketClose);
        return addPosition(arr, startToken);
    }

    MapLitExpression* Parser::parseMapLitExpr(Node* parent) {
        auto map = new MapLitExpression();
        map->parent = parent;

        auto startToken = eat(TokenType::CurlyOpen);
        while (!eof() && !match(TokenType::CurlyClose)) {
            map->keys.push_back(parseExpr(map));
            eat(TokenType::Colon);
            map->values.push_back(parseExpr(map));
            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::CurlyClose);
        return addPosition(map, startToken);
    }

    TypeAliasDeclaration* Parser::parseTypeAliasDecl(Node* parent) {
        auto alias = new TypeAliasDeclaration();
        auto startToken = eat(TokenType::Type);
        alias->_name = eat(TokenType::Identifier).value;
        eat(TokenType::Equals);
        alias->typeExpression = parseTypeExpr(alias);
        eat(TokenType::Semicolon);
        return addPosition(alias, startToken);
    }

    ClassDeclaration* Parser::parseClassDecl(Node* parent) {
        auto cls = new ClassDeclaration();
        cls->parent = parent;

        auto startToken = eat(TokenType::Class);
        cls->_name = eat(TokenType::Identifier).value;

        if (eatOptional(TokenType::LessThan)) {
            while (!eof() && !match(TokenType::GreaterThan)) {
                auto tok = eat(TokenType::Identifier);
                auto gen = new GenericParameter();
                gen->parent = cls;
                gen->_name = tok.value;
                cls->genericParams.push_back(addPosition(gen, tok));
                if (match(TokenType::Comma)) {
                    eat();
                }
            }
            eat(TokenType::GreaterThan);
        }

        eat(TokenType::CurlyOpen);
        while (!eof() && !match(TokenType::CurlyClose)) {
            if (match(TokenType::Function)) {
                cls->methods.push_back(parseFuncDecl(cls));
            }
            else if (match(TokenType::Var)) {
                auto field = parseFieldDecl(cls);
                field->index = cls->fields.size();
                cls->fields.push_back(field);
            }
            else {
                eat();
                expected("class member declaration");
            }
        }
        eat(TokenType::CurlyClose);
        return addPosition(cls, startToken);
    }

    InterfaceDeclaration* Parser::parseInterfaceDecl(Node* parent) {
        auto iface = new InterfaceDeclaration();
        iface->parent = parent;
        auto startToken = eat(TokenType::Interface);
        iface->_name = eat(TokenType::Identifier).value;
        eat(TokenType::CurlyOpen);
        while (!eof() && !match(TokenType::CurlyClose)) {
            if (match(TokenType::Function)) {
                auto im = parseInterfaceMethodDecl(iface);
                im->index = iface->methods.size();
                iface->methods.push_back(im);
            }
            else if (match(TokenType::Var)) {
                auto ifd = parseInterfaceFieldDecl(iface);
                ifd->index = iface->fields.size();
                iface->fields.push_back(ifd);
            }
            else {
                eat();
                expected("Interface member declaration.");
            }
        }
        eat(TokenType::CurlyClose);
        return addPosition(iface, startToken);
    }

    InterfaceMethodDeclaration* Parser::parseInterfaceMethodDecl(Node* parent) {
        auto im = new InterfaceMethodDeclaration();
        im->parent = parent;
        auto startToken = eat(TokenType::Function);
        im->name = eat(TokenType::Identifier).value;
        eat(TokenType::ParenOpen);
        while (!eof() && !match(TokenType::ParenClose)) {
            auto param = parseParam(im);
            param->index = im->parameters.size();
            im->parameters.push_back(param);
            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::ParenClose);
        if (eatOptional(TokenType::Colon)) {
            im->returnTypeExpression = parseTypeExpr(im);
        }
        eat(TokenType::Semicolon);
        
        return addPosition(im, startToken);
    }
    
    InterfaceFieldDeclaration* Parser::parseInterfaceFieldDecl(Node* parent) {
        auto ifd = new InterfaceFieldDeclaration();
        ifd->parent = parent;
        auto startToken = eat(TokenType::Var);
        ifd->name = eat(TokenType::Identifier).value;
        eat(TokenType::Colon);
        ifd->typeExpression = parseTypeExpr(ifd);
        eat(TokenType::Semicolon);
        
        return addPosition(ifd, startToken);
    }

    EnumDeclaration* Parser::parseEnumDecl(Node* parent) {
        auto en = new EnumDeclaration();
        en->parent = parent;
        auto startToken = eat(TokenType::Enum);
        en->_name = eat(TokenType::Identifier).value;
        eat(TokenType::CurlyOpen);
        while (match(TokenType::Identifier)) {
            auto tok = eat(TokenType::Identifier);
            auto ee = new EnumElement();
            ee->parent = en;
            ee->name = tok.value;
            auto el = addPosition(ee, tok);
            el->index = en->elements.size();
            en->elements.push_back(el);

            if (!eatOptional(TokenType::Comma)) {
                break;
            }
        }
        eat(TokenType::CurlyClose);
        return addPosition(en, startToken);
    }

    bool Parser::match(TokenType type) {
        return !eof() && token->type == type;
    }

    bool Parser::matchExpr() {
        return
            match(TokenType::BracketOpen) ||
            match(TokenType::CurlyOpen) ||
            match(TokenType::This) ||
            match(TokenType::Null) ||
            match(TokenType::Integer) ||
            match(TokenType::Float) ||
            match(TokenType::String) ||
            match(TokenType::Boolean) ||
            match(TokenType::Identifier) ||
            match(TokenType::ParenOpen) ||
            match(TokenType::New) || 
            matchUnary()
            ;
    }

    bool Parser::matchUnary() {
        return
            match(TokenType::Minus) ||
            match(TokenType::ExclamationMark) ||
            match(TokenType::Tilde) ||
            match(TokenType::MinusMinus) ||
            match(TokenType::PlusPlus)
            ;
    }

    bool Parser::matchSecondary() {
        return
            match(TokenType::ParenOpen) ||
            match(TokenType::Period) ||
            match(TokenType::MinusMinus) ||
            match(TokenType::PlusPlus) ||
            match(TokenType::Is) ||
            match(TokenType::BracketOpen) ||
            match(TokenType::As) ||
            matchBinary();
    }

    bool Parser::matchBinary() {
        return
            match(TokenType::Plus) ||
            match(TokenType::Minus) ||
            match(TokenType::Asterisk) ||
            match(TokenType::Slash) ||
            match(TokenType::Percent) ||
            match(TokenType::Equals) ||
            match(TokenType::PlusEquals) ||
            match(TokenType::MinusEquals) ||
            match(TokenType::AsteriskEquals) ||
            match(TokenType::SlashEquals) ||
            match(TokenType::EqualsEquals) ||
            match(TokenType::ExclamationMarkEquals) ||
            match(TokenType::AmpAmp) ||
            match(TokenType::PipePipe) ||
            match(TokenType::LessThan) ||
            match(TokenType::GreaterThan) ||
            match(TokenType::LessThanEquals) ||
            match(TokenType::GreaterThanEquals)
            ;
    }

    bool Parser::matchTypeSecondary() {
        return
            match(TokenType::BracketOpen) ||
            match(TokenType::LessThan) ||
            match(TokenType::Pipe) ||
            match(TokenType::QuestionMark) ||
            match(TokenType::Period)
            ;
    }

    void Parser::expect(TokenType type) {
        if (!match(type)) {
            expected(type);
        }
    }

    Token Parser::eat(TokenType type) {
        expect(type);
        return eat();
    }
    
    bool Parser::eatOptional(TokenType type) {
        if (match(type)) {
            eat();
            return true;
        }
        return false;
    }
    
    Token Parser::eat() {
        if (eof()) return *token;
        else return *token++;
    }

    bool Parser::eof() {
        return token == tokens.end();
    }

    void Parser::expected(TokenType expectedType) {
        error(source, *token, "Unexpected '" + getTokenName(token->type) + "', expected " + getTokenName(expectedType) + ".");
    }

    void Parser::expected(const std::string& expected) {
        error(source, *token, "Unexpected '" + getTokenName(token->type) + "', expected " + expected + ".");
    }
}