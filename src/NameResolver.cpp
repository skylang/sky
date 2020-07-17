// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "NameResolver.h"
#include "ast/nodes.h"
#include "exceptions.h"
#include "Scope.h"
#include "SourceFile.h"

namespace Sky {
    NameResolver::NameResolver(Scope* globals): scope(globals) {
    }

    int NameResolver::resolveGenerics(ModuleDeclaration& n) {
        int numGenerics = 0;

        auto oldscope = scope;
        scope = new Scope(scope);

        for (auto&& imp: n.imports) {
            if (imp->all) {
                for (auto&& fun: imp->module->functions) {
                    if (fun->isExported) {
                        scope->add(fun->name, fun);
                    }
                }
                for (auto&& cls: imp->module->classes) {
                    if (cls->isExported) {
                        scope->add(cls->_name, cls);
                    }
                }
                for (auto&& iface: imp->module->interfaces) {
                    if (iface->isExported) {
                        scope->add(iface->_name, iface);
                    }
                }
                for (auto&& en: imp->module->enums) {
                    if (en->isExported) {
                        scope->add(en->_name, en);
                    }
                }
            }
            else if (imp->importModule) {
                scope->add(imp->parts.back(), imp->module);
            }
            else {
                auto cls = imp->module->getClass(imp->parts.back());
                if (cls) {
                    if (cls->isExported) {
                        scope->add(cls->_name, cls);
                        continue;
                    }
                    else {
                        throw Exception(cls->_name + " is not exported.");
                    }
                }

                auto en = imp->module->getEnum(imp->parts.back());
                if (en) {
                    if (en->isExported) {
                        scope->add(en->_name, en);
                        continue;
                    }
                    else {
                        throw Exception(en->_name + " is not exported.");
                    }
                }

                auto funs = imp->module->getFunctions(imp->parts.back());
                if (!funs.empty()) {
                    for (auto&& fun: funs) {
                        if (fun->isExported) {
                            scope->add(fun->name, fun);
                        }
                    }
                }
            }
        }

        for (auto&& fun: n.functions) {
            scope->add(fun->name, fun);
        }
        for (auto&& cls: n.classes) {
            scope->add(cls->_name, cls);
        }
        for (auto&& iface: n.interfaces) {
            scope->add(iface->_name, iface);
        }
        for (auto&& en: n.enums) {
            scope->add(en->_name, en);
        }
        for (auto&& ta: n.typeAliases) {
            scope->add(ta->_name, ta);
        }

        for (auto& cls: n.classes) {
            for (auto& gen: cls->reifiedClasses) {
                if (!gen->isResolved) {
                    resolve(*gen);
                    numGenerics++;
                }
            }
        }

        delete scope;
        scope = oldscope;
        
        return numGenerics;
    }

    void NameResolver::resolve(ModuleDeclaration& n) {
        auto oldscope = scope;
        scope = new Scope(scope);

        for (auto& import: n.imports) {
            resolve(*import);
        }

        for (auto&& fun: n.functions) {
            scope->add(fun->name, fun);
        }
        for (auto&& cls: n.classes) {
            scope->add(cls->_name, cls);
        }
        for (auto&& iface: n.interfaces) {
            scope->add(iface->_name, iface);
        }
        for (auto&& en: n.enums) {
            scope->add(en->_name, en);
        }
        for (auto&& ta: n.typeAliases) {
            scope->add(ta->_name, ta);
        }

        for (auto&& fun: n.functions) {
            resolve(*fun);
        }
        for (auto&& iface: n.interfaces) {
            resolve(*iface);
        }
        for (auto&& cls: n.classes) {
            resolve(*cls);
        }
        for (auto&& ta: n.typeAliases) {
            resolve(*ta);
        }

        delete scope;
        scope = oldscope;
    }

    void NameResolver::resolve(ClassDeclaration& n) {
        // is generic
        if (n.genericParams.size() > 0 && n.genericArguments.empty()) return;

        // is reified generic
        if (n.genericParams.size() > 0 && n.genericArguments.size() > 0) {
            if (n.isResolved) return;
            n.isResolved = true;
        }

        auto oldscope = scope;
        scope = new Scope(scope);

        for (int i = 0; i < n.genericParams.size(); ++i) {
            scope->add(n.genericParams[i]->_name, n.genericArguments[i]);
        }

        for (auto&& field: n.fields) {
            resolve(*field);
        }
        for (auto&& method: n.methods) {
            resolve(*method);
        }

        delete scope;
        scope = oldscope;
    }

    void NameResolver::resolve(FunctionDeclaration& n) {
        auto oldscope = scope;
        scope = new Scope(scope);

        if (n.returnTypeExpression) visitChild(n.returnTypeExpression);
        for (auto& param: n.parameters) {
            resolve(*param);
        }
        visitChildren(n.statements);

        std::vector<TypeDeclaration*> parameterTypes;
        for (auto&& param: n.parameters) {
            parameterTypes.push_back(param->typeExpression->typeValue);
        }
        n.declarationType = FunctionType::get(n.returnTypeExpression ? n.returnTypeExpression->typeValue : &VoidType::instance, parameterTypes);

        delete scope;
        scope = oldscope;
    }

    void NameResolver::resolve(Parameter& n) {
        visitChild(n.typeExpression);
        n.declarationType = n.typeExpression->typeValue;
        scope->add(n.name, &n);
    }

    void NameResolver::visit(VarDeclaration& n) {
        if (n.typeExpression) {
            visitChild(n.typeExpression);
            n.declarationType = n.typeExpression->typeValue;
        }

        if (n.initializer) {
            visitChild(n.initializer);
        }

        try {
            scope->add(n.name, &n);
        }
        catch (const DuplicateSymbolException& e) {
            error(n, e.what());
        }
    }

    void NameResolver::visit(IsExpression& n) {
        visitChild(n.target);
        visitChild(n.typeExpression);
    }

    void NameResolver::visit(UnionTypeExpression& n) {
        visitChild(n.base);
        visitChild(n.next);
        n.type = &TypeType::instance;
        n.typeValue = UnionType::get(n.base->typeValue, n.next->typeValue);
    }

    void NameResolver::visit(IdExpression& n) {
        auto symbol = scope->find(n.name);
        if (!symbol) {
            error(n, "Unresolved symbol '" + n.name + "'.");
            return;
        }

        if (auto td = symbol->node->as<TypeDeclaration>()) {
            n.type = &TypeType::instance;
            n.typeValue = td;
        }
        else if (auto var = symbol->node->as<VarDeclaration>()) {
            n.node = var;
            n.type = var->declarationType;
        }
        else if (auto par = symbol->node->as<Parameter>()) {
            n.node = par;
            n.type = par->declarationType;
        }
        else if (auto fun = symbol->node->as<FunctionDeclaration>()) {
            if (symbol->next) {
                auto cur = symbol;
                while (cur) {
                    n.candidates.push_back(cur->node->as<FunctionDeclaration>());
                    cur = cur->next;
                }
                n.type = &OverloadedFunctionType::instance;
                n.node = nullptr;
            }
            else {
                n.node = fun;
                n.type = fun->declarationType;
            }
        }
        else {
            error(n, "Unhandled symbol kind.");
            return;
        }
    }

    void NameResolver::visit(ReturnStatement& n) {
        if (n.expression) {
            visitChild(n.expression);
        }
    }

    void NameResolver::visit(ExpressionStatement& n) {
        visitChild(n.expression);
    }

    void NameResolver::visit(CallExpression& n) {
        visitChild(n.callTarget);
        visitChildren(n.arguments);
    }

    void NameResolver::visit(BlockStatement& n) {
        auto oldscope = scope;
        scope = new Scope(scope);

        visitChildren(n.statements);
        
        delete scope;
        scope = oldscope;
    }

    void NameResolver::visit(BinopExpression& n) {
        visitChild(n.left);
        visitChild(n.right);
    }

    void NameResolver::visit(CastExpression& n) {
        visitChild(n.sourceExpression);
        visitChild(n.targetTypeExpression);
    }

    void NameResolver::visit(ScopeExpression& n) {
        visitChild(n.scopeTarget);
    }

    void NameResolver::visit(IfStatement& n) {
        visitChild(n.condition);
        visitChild(n.trueBranch);
        if (n.falseBranch) {
            visitChild(n.falseBranch);
        }
    }

    void NameResolver::resolve(FieldDeclaration& n) {
        visitChild(n.typeExpression);
        n.declarationType = n.typeExpression->typeValue;
    }

    void NameResolver::visit(NewExpression& n) {
        visitChild(n.typeExpression);
        visitChildren(n.arguments);
    }

    void NameResolver::visit(AssignExpression& n) {
        visitChild(n.left);
        visitChild(n.right);
    }

    void NameResolver::visit(WhileStatement& n) {
        visitChild(n.condition);
        visitChild(n.body);
    }

    void NameResolver::visit(PostfixExpression& n) {
        visitChild(n.target);
    }

    void NameResolver::visit(ArrayTypeExpression& n) {
        visitChild(n.baseTypeExpression);
        n.type = &TypeType::instance;
        if (n.baseTypeExpression->typeValue == &InvalidType::instance) {
            return;
        }
        n.typeValue = ArrayType::get(n.baseTypeExpression->typeValue);
    }

    void NameResolver::resolve(ImportStatement& n) {
        if (n.all) {
            for (auto&& fun: n.module->functions) {
                if (fun->isExported) {
                    scope->add(fun->name, fun);
                }
            }
            for (auto&& cls: n.module->classes) {
                if (cls->isExported) {
                    scope->add(cls->_name, cls);
                }
            }
            for (auto&& iface: n.module->interfaces) {
                if (iface->isExported) {
                    scope->add(iface->_name, iface);
                }
            }
            for (auto&& en: n.module->enums) {
                if (en->isExported) {
                    scope->add(en->_name, en);
                }
            }
            for (auto&& alias: n.module->typeAliases) {
                if (alias->isExported) {
                    scope->add(alias->_name, alias);
                }
            }
        }
        else if (n.importModule) {
            scope->add(n.parts.back(), n.module);
        }
        else {
            auto cls = n.module->getClass(n.parts.back());
            if (cls) {
                if (cls->isExported) {
                    scope->add(cls->_name, cls);
                    return;
                }
            }

            auto en = n.module->getEnum(n.parts.back());
            if (en) {
                if (en->isExported) {
                    scope->add(en->_name, en);
                    return;
                }
            }

            auto funs = n.module->getFunctions(n.parts.back());
            if (!funs.empty()) {
                for (auto&& fun: funs) {
                    if (fun->isExported) {
                        scope->add(fun->name, fun);
                    }
                }
                return;
            }

            auto alias = n.module->getAlias(n.parts.back());
            if (alias) {
                if (alias->isExported) {
                    scope->add(alias->_name, alias);
                    return;
                }
            }

            // nothing found
            error(n, "Symbol '" + n.parts.back() + "' not found in module '" + n.getBaseName() + "'");
        }
    }

    void NameResolver::visit(UnaryExpression& n) {
        visitChild(n.target);
    }
    
    void NameResolver::visit(ArrayLitExpression& n) {
        visitChildren(n.elements);
    }

    void NameResolver::visit(MapLitExpression& n) {
        visitChildren(n.keys);
        visitChildren(n.values);
    }

    void NameResolver::visit(SubscriptExpression& n) {
        visitChildren(n.arguments);
        visitChild(n.callTarget);
    }

    void NameResolver::resolve(InterfaceDeclaration& n) {
        for (auto& field: n.fields) {
            resolve(*field);
        }
        for (auto& method: n.methods) {
            resolve(*method);
        }
    }

    void NameResolver::visit(NullableTypeExpression& n) {
        visitChild(n.baseTypeExpression);
        n.type = &TypeType::instance;
        n.typeValue = UnionType::get(n.baseTypeExpression->typeValue, &NullType::instance);
    }

    void NameResolver::visit(GenericReificationExpression& n) {
        visitChild(n.baseTypeExpression);
        visitChildren(n.genericArguments);

        auto cls = n.baseTypeExpression->typeValue->as<ClassDeclaration>();
        if (!cls) {
            error(n, "Can not reify non-class type '" + n.baseTypeExpression->typeValue->getFullName() + "'.");
            return;
        }

        if (cls->genericParams.empty()) {
            error(n, "Can not reify non-generic class '" + n.baseTypeExpression->typeValue->getFullName() + "'.");
            return;
        }

        if (cls->genericParams.size() != n.genericArguments.size()) {
            error(n, "Expected " + std::to_string(cls->genericParams.size()) + " type arguments, " + std::to_string(n.genericArguments.size()) + " given.");
            return;
        }

        std::vector<TypeDeclaration*> types;
        for (auto&& tex: n.genericArguments) {
            types.push_back(tex->typeValue);
        }

        n.type = &TypeType::instance;
        n.typeValue = cls->getReifiedClass(types);
    }

    void NameResolver::resolve(InterfaceFieldDeclaration& n) {
        visitChild(n.typeExpression);
        n.declarationType = n.typeExpression->typeValue;
    }

    void NameResolver::resolve(InterfaceMethodDeclaration& n) {
        visitChild(n.returnTypeExpression);
        for (auto& param: n.parameters) {
            resolve(*param);
        }

        std::vector<TypeDeclaration*> parameterTypes;
        for (auto&& param: n.parameters) {
            parameterTypes.push_back(param->typeExpression->typeValue);
        }
        n.type = FunctionType::get(n.returnTypeExpression ? n.returnTypeExpression->typeValue : &VoidType::instance, parameterTypes);
    }

    void NameResolver::resolve(TypeAliasDeclaration& n) {
        visitChild(n.typeExpression);
    }
}