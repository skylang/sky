// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "ClassDeclaration.h"
#include "FunctionDeclaration.h"
#include "FieldDeclaration.h"
#include "../Parser.h"

namespace Sky {
    Node* ClassDeclaration::getMember(const std::string& name) {
        for (auto&& method: methods) {
            if (method->name == name) {
                return method;
            }
        }
        for (auto&& field: fields) {
            if (field->name == name) {
                return field;
            }
        }
        return nullptr;
    }

    std::vector<Node*> ClassDeclaration::getMethods(const std::string& name) {
        std::vector<Node*> met;
        for (auto&& method: methods) {
            if (method->name == name) {
                met.push_back(method);
            }
        }
        return met;
    }

    ClassDeclaration* ClassDeclaration::getReifiedClass(const std::vector<TypeDeclaration*>& typeArgs) {
        for (auto& rc: reifiedClasses) {
            bool found = true;
            for (int i = 0; i < typeArgs.size(); ++i) {
                if (typeArgs[i] != rc->genericArguments[i]) {
                    found = false;
                    break;
                }
            }
            if (found)  {
                return rc;
            }
        }

        // copy class definition
        Parser parser(*source, firstToken);
        auto cls = parser.parseClassDecl(parent);
        cls->genericBase = this;
        // set type arguments
        cls->genericArguments = typeArgs;
        cls->_name += "<";
        for (auto&& t: typeArgs) {
            cls->_name += t->getFullName();
            if (&t != &typeArgs.back()) {
                cls->_name += ",";
            }
        }
        cls->_name += ">";
        reifiedClasses.push_back(cls);
        return cls;
    }
}