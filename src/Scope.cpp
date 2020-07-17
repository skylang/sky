// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "Scope.h"
#include "exceptions.h"
#include "ast/FunctionDeclaration.h"

namespace Sky {
    Scope* Scope::getParent() {
        return parent;
    }

    Scope::~Scope() {
        for (auto&& it: symbols) {
           //delete it.second;
        }
    }

    void Scope::add(const std::string& name, Node* node) {
        add(name, new Symbol(node));
    }

    void Scope::add(const std::string& name, Symbol* symbol) {
        auto it = symbols.find(name);
        if (it != symbols.end()) {
			if (symbol->node->as<FunctionDeclaration>() && it->second->node->as<FunctionDeclaration>()) {
				symbol->next = it->second;
				it->second = symbol;
			}
			else {
				throw DuplicateSymbolException("Duplicate symbol " + name);
			}
        }
        
        symbols.insert(std::make_pair(name, symbol));
    }

    Symbol* Scope::find(const std::string& name) {
        auto it = symbols.find(name);
        if (it != symbols.end()) return it->second;
        if (parent) return parent->find(name);
        return nullptr;
    }

    void Scope::setParent(Scope* p) {
        parent = p;
    }
}