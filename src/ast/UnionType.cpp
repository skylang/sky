// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#include "UnionType.h"
#include "../exceptions.h"
#include "TypeAliasDeclaration.h"
#include "Expression.h"

#include <sstream>

namespace Sky {
    std::set<TypeDeclaration*> mergeTypes(TypeDeclaration* left, TypeDeclaration* right) {
        std::set<TypeDeclaration*> set;
        if (auto unionLeft = left->as<UnionType>()) {
            set.insert(unionLeft->containedTypes.begin(), unionLeft->containedTypes.end());
        }
        else {
            set.insert(left);
        }
        if (auto unionRight = right->as<UnionType>()) {
            set.insert(unionRight->containedTypes.begin(), unionRight->containedTypes.end());
        }
        else {
            set.insert(right);
        }
        return set;
    }

    std::string getUnionName(const UnionType* unionType) {
        std::stringstream sstr;
        for (auto&& type: unionType->containedTypes) {
            if (&type != &*unionType->containedTypes.begin()) {
                sstr << "|";
            }
            if (auto ut = type->as<UnionType>()) {
                if (ut == unionType) {
                    sstr << "*self*";
                    continue;
                }
            }
            sstr << type->getFullName();
        }
        return sstr.str();
    }
    
    int UnionType::getTypeTag(TypeDeclaration* t) {
        if (auto alias = t->as<TypeAliasDeclaration>()) {
            t = alias->typeExpression->typeValue;
        }

        int tag = 0;
        for (auto&& type: containedTypes) {
            if (type == t) return tag;
            if (auto alias = type->as<TypeAliasDeclaration>()) {
                if (alias->typeExpression->typeValue == t) {
                    return tag;
                }
            }
            ++tag;
        }
        return -1; // TODO: error handling
    }

    bool UnionType::containsType(TypeDeclaration* t) {
        for (auto& ct: containedTypes) {
            if (ct == t) {
                return true;
            }
            if (auto alias = ct->as<TypeAliasDeclaration>()) {
                if (alias->typeExpression->typeValue == t) {
                    return true;
                }
            }
        }
        return false;
    }

    TypeDeclaration* UnionType::getComplementaryType(TypeDeclaration* t) {
        if (containedTypes.size() != 2) throw Exception("Calling UnionType::getComplementaryType() on union without exactly 2 contained types.");
        for (auto& ct: containedTypes) {
            if (ct != t) return ct;
        }
        return nullptr; // TODO: error handling
    }

    UnionType* UnionType::get(TypeDeclaration* left, TypeDeclaration* right) {
        auto set = mergeTypes(left, right);
        for (auto& un: unionTypes) {
            if (un->containedTypes == set) {
                return un;
            }
        }

        auto ut = new UnionType();
        ut->containedTypes = set;
        ut->_name = getUnionName(ut);
        unionTypes.push_back(ut);
        return ut;
    }

    std::vector<UnionType*> UnionType::unionTypes;
}