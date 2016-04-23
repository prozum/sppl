//
// Created by hejsil on 4/21/16.
//

#include "GenericUnfolder.h"

namespace semantics {
    void GenericUnfolder::visit(Program &Node) {
        Prog = &Node;

        findGenerics();

        for (auto &Decl: Node.Decls) {
            Decl->accept(*this);
        }

        removeGenerics();
    }

    void GenericUnfolder::findGenerics() {
        for (auto &Decl: Prog->Decls) {
            if (typeid(*Decl) == typeid(Function)) {
                auto Func = (Function*)Decl.get();

                if (isGeneric(*Func)) {
                    if (GenericFuncs.find(Func->Id) == GenericFuncs.end()) {
                        GenericFuncs.insert({ Func->Id, *Func });
                    } else {
                        throw Error(Func->Id + " was declared twice", Func->Loc);
                    }
                }
            } else if (typeid(*Decl) == typeid(AlgebraicDT)) {
                auto ADT = (AlgebraicDT*)Decl.get();

                if (isGeneric(*ADT)) {
                    if (GenericTypes.find(ADT->Name) == GenericTypes.end()) {
                        GenericTypes.insert({ ADT->Name, *ADT });
                    } else {
                        throw Error(ADT->Name + " was declared twice", ADT->Loc);
                    }
                }
            }
        }
    }

    void GenericUnfolder::removeGenerics() {
        for (size_t i = 0; i < Prog->Decls.size();) {
            if (typeid(*Prog->Decls[i]) == typeid(Function)) {
                auto Func = (Function*)Prog->Decls[i].get();

                if (GenericFuncs.find(Func->Id) != GenericFuncs.end()) {
                    Prog->Decls.erase(Prog->Decls.begin() + (i));
                } else {
                    i++;
                }
            } else if (typeid(*Prog->Decls[i]) == typeid(AlgebraicDT)) {
                auto ADT = (AlgebraicDT*)Prog->Decls[i].get();

                if (GenericTypes.find(ADT->Name) != GenericTypes.end()) {
                    Prog->Decls.erase(Prog->Decls.begin() + (i));
                } else {
                    i++;
                }
            }
        }
    }

    bool GenericUnfolder::isGeneric(Function &Func) {
        for (auto &Ty: Func.Signature.Subtypes) {
            if (isGeneric(Ty))
                return true;
        }

        return false;
    }

    bool GenericUnfolder::isGeneric(AlgebraicDT &ADT) {
        return ADT.TypeConstructor.size() > 0;
    }

    bool GenericUnfolder::isGeneric(Type &Ty) {
        switch (Ty.Id) {
            case TypeId::GENERIC:
                return true;
            case TypeId::LIST:
            case TypeId::SIGNATURE:
            case TypeId::TUPLE:
            case TypeId::CUSTOM:
                for (auto &Child: Ty.Subtypes) {
                    if (isGeneric(Child))
                        return true;
                }
            default:
                return false;
        }
    }

    void GenericUnfolder::visit(Function &Node) {
        CurrentFunc = &Node;

        for (auto &Case: Node.Cases) {
            Case->accept(*this);
        }
    }

    void GenericUnfolder::visit(AlgebraicDT &Node) {
        for (auto &Product: Node.Sum) {
            Product->accept(*this);
        }
    }

    void GenericUnfolder::visit(Product &Node) {

    }

    void GenericUnfolder::visit(Case &Node) {
        for (auto &Pattern: Node.Patterns) {

            Pattern->accept(*this);
        }
        Node.Expr->accept(*this);
    }

    void GenericUnfolder::visit(LambdaArg &Node) {
        
    }

    void GenericUnfolder::visit(Or &Node) {
        
    }

    void GenericUnfolder::visit(And &Node) {
        
    }

    void GenericUnfolder::visit(Equal &Node) {
        
    }

    void GenericUnfolder::visit(NotEqual &Node) {
        
    }

    void GenericUnfolder::visit(Lesser &Node) {
        
    }

    void GenericUnfolder::visit(Greater &Node) {
        
    }

    void GenericUnfolder::visit(LesserEq &Node) {
        
    }

    void GenericUnfolder::visit(GreaterEq &Node) {
        
    }

    void GenericUnfolder::visit(Add &Node) {
        
    }

    void GenericUnfolder::visit(Sub &Node) {
        
    }

    void GenericUnfolder::visit(Mul &Node) {
        
    }

    void GenericUnfolder::visit(Div &Node) {
        
    }

    void GenericUnfolder::visit(Mod &Node) {
        
    }

    void GenericUnfolder::visit(ListAdd &Node) {
        
    }

    void GenericUnfolder::visit(ProducerConsumer &Node) {
        
    }

    void GenericUnfolder::visit(Concat &Node) {
        
    }

    void GenericUnfolder::visit(To &Node) {
        
    }

    void GenericUnfolder::visit(ParExpr &Node) {
        
    }

    void GenericUnfolder::visit(Not &Node) {
        
    }

    void GenericUnfolder::visit(Negative &Node) {
        
    }

    void GenericUnfolder::visit(LambdaFunction &Node) {
        
    }

    void GenericUnfolder::visit(IdPattern &Node) {
        
    }

    void GenericUnfolder::visit(IntPattern &Node) {
        
    }

    void GenericUnfolder::visit(FloatPattern &Node) {
        
    }

    void GenericUnfolder::visit(CharPattern &Node) {
        
    }

    void GenericUnfolder::visit(BoolPattern &Node) {
        
    }

    void GenericUnfolder::visit(StringPattern &Node) {
        
    }

    void GenericUnfolder::visit(ListPattern &Node) {
        
    }

    void GenericUnfolder::visit(TuplePattern &Node) {
        
    }

    void GenericUnfolder::visit(ListSplit &Node) {
        
    }

    void GenericUnfolder::visit(WildPattern &Node) {
        
    }

    void GenericUnfolder::visit(AlgebraicPattern &Node) {
        
    }

    void GenericUnfolder::visit(ParPattern &Node) {
        
    }

    void GenericUnfolder::visit(IdExpr &Node) {
        
    }

    void GenericUnfolder::visit(IntExpr &Node) {
        
    }

    void GenericUnfolder::visit(FloatExpr &Node) {
        
    }

    void GenericUnfolder::visit(CharExpr &Node) {
        
    }

    void GenericUnfolder::visit(BoolExpr &Node) {
        
    }

    void GenericUnfolder::visit(ListExpr &Node) {
        
    }

    void GenericUnfolder::visit(TupleExpr &Node) {
        
    }

    void GenericUnfolder::visit(CallExpr &Node) {
        
    }

    void GenericUnfolder::visit(AlgebraicExpression &Node) {
        
    }

}