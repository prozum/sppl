#include "Scope.h"
#include "ScopeGenerator.h"
#include <iostream>

using namespace std;
using namespace common;
using namespace semantics;

ScopeGenerator::ScopeGenerator(Scope *Scp) : CurScope(Scp) {}

void ScopeGenerator::visit(Program &Node) {
    /* This is code for getting all the generics out of the program.
     * It is not in use, but don't delete it yet.
    for (size_t i = 0; i < Node.Decls.size();) {
        auto &Decl = Node.Decls[i];

        if (typeid(*Decl) == typeid(Function)) {
            auto Func = (Function *)Decl.get();

            if (isGeneric(*Func)) {
                throw runtime_error(
                    "Generics not supported yet. Plz don't use them!");

                if (CurScope->GenericFuncs.find(Func->Id) ==
                    CurScope->GenericFuncs.end()) {
                    auto Ptr = Decl.release();
                    CurScope->GenericFuncs[Func->Id] =
                        unique_ptr<Function>((Function *)Ptr);
                    CurScope->GeneratedGenerics[Func->Id] =
                        unordered_map<Type, string>();
                    Node.Decls.erase(Node.Decls.begin() + i);
                } else {
                    throw Error(Func->Id + " was declared twice", Func->Loc);
                }
            } else {
                i++;
            }
        } else if (typeid(*Decl) == typeid(AlgebraicDT)) {
            auto ADT = (AlgebraicDT *)Decl.get();

            if (isGeneric(*ADT)) {
                throw runtime_error(
                    "Generics not supported yet. Plz don't use them!");

                if (CurScope->GenericADT.find(ADT->Name) ==
                    CurScope->GenericADT.end()) {
                    auto Ptr = Decl.release();
                    CurScope->GenericADT[ADT->Name] =
                        unique_ptr<AlgebraicDT>((AlgebraicDT *)Ptr);
                    CurScope->GeneratedGenerics[ADT->Name] =
                        unordered_map<Type, string>();
                    Node.Decls.erase(Node.Decls.begin() + i);
                } else {
                    throw Error(ADT->Name + " was declared twice", ADT->Loc);
                }
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
     */

    for (auto &Func : Node.Decls) {
        Func->accept(*this);
    }
}

bool ScopeGenerator::isGeneric(Function &Func) {
    for (auto &Ty : Func.Signature.Subtypes) {
        if (isGeneric(Ty))
            return true;
    }

    return false;
}

bool ScopeGenerator::isGeneric(AlgebraicDT &ADT) {
    return ADT.TypeConstructor.size() > 0;
}

bool ScopeGenerator::isGeneric(Type &Ty) {
    switch (Ty.Id) {
    case TypeId::GENERIC:
        return true;
    case TypeId::LIST:
    case TypeId::SIGNATURE:
    case TypeId::TUPLE:
    case TypeId::CUSTOM:
        for (auto &Child : Ty.Subtypes) {
            if (isGeneric(Child))
                return true;
        }
    default:
        return false;
    }
}

void ScopeGenerator::visit(Function &Node) {
    Type Out;

    Node.Scp = CurScope;

    if (!CurScope->tryGetDecl(Node.Id, Out) || Node.Anon) {
        CurScope->Decls.insert({Node.Id, Node.Signature});
    } else {
        addError(Error(Node.Id + " has already been declared", Node.Loc));
        return;
    }
}

void ScopeGenerator::visit(AlgebraicDT &Node) {
    AlgebraicDT *Out;
    CurADT = &Node;

    if (!CurScope->tryGetADT(Node.Name, Out)) {
        CurScope->Types.insert({Node.Name, Node});

        for (auto &Product : Node.Sum) {
            Product->accept(*this);
        }
    } else {
        addError(Error(Node.Name + " has already been declared", Node.Loc));
        return;
    }
}

void ScopeGenerator::visit(Product &Node) {
    Product *Out;

    Node.Parent = CurADT;

    if (!CurScope->tryGetCon(Node.Constructor, Out)) {
        CurScope->Constructors.insert({Node.Constructor, Node});
    } else {
        addError(Error(Node.Constructor + " has already been declared", Node.Loc));
        return;
    }
}
