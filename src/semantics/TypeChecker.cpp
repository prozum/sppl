#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"


using namespace std;
namespace semantics
{
    TypeChecker::TypeChecker() { }

    void TypeChecker::visit(Program &Node) {
        /*
        auto StrList = Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }));

        // Visit children
        for (auto &Func : Node.Decls) {
            Func->accept(*this);

            try {
                if (Func->Id == "main") {
                    if (Func->Signature.Subtypes.front() != StrList) {
                        throw Error::Expected("Declaration of \"main\" had wrong input type",
                                              StrList.str(),
                                              Func->Signature.Subtypes.front().str(),
                                              Func->Loc);
                    } else if (Func->Signature.Subtypes.size() != 2) {
                        throw Error::Expected("Function \"main\" had wrong number of input",
                                              "2",
                                              to_string(Func->Signature.Subtypes.size()),
                                              Func->Loc);
                    }
                }
            }
            catch (Error err) {
                Errors.push_back(err);
            }

        }
        // Visit stops here
         */
    }

    void TypeChecker::visit(Function &Node) {
        CurFunc = &Node;

        // Visit children
        for (auto &Case : Node.Cases) {
            try {
                Case->accept(*this);
            }
            catch (Error err) {
                Errors.push_back(err);
            }
        }
        // Visit stops here

        // Set return type
        if (!hasError())
            Node.RetTy = Node.Signature.Subtypes.back();
    }

    void TypeChecker::visit(Case &Node) {
        // Visit children
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }
        Node.Expr->accept(*this);
        // Visit stops here

        // Set signature for anonymous function
        if (CurFunc->Anon) {
            CurFunc->Signature = Type(TypeId::SIGNATURE, vector<Type>({Node.Expr->RetTy}));
        }

        if (Node.Patterns.size() != CurFunc->Signature.Subtypes.size() - 1) {
            throw Error::Expected("Wrong pattern count",
                                  to_string(CurFunc->Signature.Subtypes.size() - 1),
                                  to_string(Node.Patterns.size()),
                                  Node.Loc);
        }

        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            if (Node.Patterns[i]->RetTy.Id == TypeId::EMPTYLIST &&
                    CurFunc->Signature.Subtypes[i].Id == TypeId::LIST) {
                Node.Patterns[i]->RetTy = CurFunc->Signature.Subtypes[i];
            }

            if (Node.Patterns[i]->RetTy != CurFunc->Signature.Subtypes[i]) {
                throw Error::Expected("Wrong pattern type",
                                         CurFunc->Signature.Subtypes[i].str(),
                                         Node.Patterns[i]->RetTy.str(),
                                         Node.Patterns[i]->Loc);
            }
        }

        if (Node.Expr->RetTy.Id == TypeId::EMPTYLIST &&
                CurFunc->Signature.Subtypes.back().Id == TypeId::LIST)
            Node.Expr->RetTy = CurFunc->Signature.Subtypes.back();
        else if (CurFunc->Signature.Subtypes.back() != Node.Expr->RetTy) {
            throw Error::Expected("Wrong return type",
                                     CurFunc->RetTy.str(),
                                     Node.Expr->RetTy.str(),
                                     Node.Loc);
        }
    }

    void TypeChecker::visit(Or &Node) {
        /*
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->RetTy.Id == TypeId::BOOL && Node.Right->RetTy.Id == TypeId::BOOL) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   Node);
        }
        */
    }

    void TypeChecker::visit(And &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->RetTy == TypeId::BOOL && Node.Right->RetTy == TypeId::BOOL) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   Node);
        }
    }

    void TypeChecker::visit(Equal &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->RetTy == Node.Right->RetTy) {
            Node.RetTy = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   Node);
        }
    }

    void TypeChecker::visit(NotEqual &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->RetTy == Node.Right->RetTy) {
            Node.RetTy = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   Node);
        }
    }

    void TypeChecker::visit(Lesser &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   Node);
        }
    }

    void TypeChecker::visit(Greater &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(LesserEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(GreaterEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(Add &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(Sub &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(Mul &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(Div &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(Mod &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT) ||
            (Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT)) {
            Node.RetTy = Node.Left->RetTy;
        } else {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                   Node);
        }
    }

    void TypeChecker::visit(ListAdd &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Right->RetTy.Id == TypeId::LIST) {
            if (Node.Left->RetTy == Node.Right->RetTy.Subtypes.front()) {
                Node.RetTy = Node.Right->RetTy;
            } else {
                throw Error::Binary("Left type must be same type of right ListExpression",
                                       Node);
            }
        } else if (Node.Right->RetTy.Id == TypeId::STRING) {
            if (Node.Left->RetTy.Id == TypeId::CHAR) {
                Node.RetTy = Node.Right->RetTy;
            } else {
                throw Error::Binary("Left type must be CharPattern when right is String",
                                       Node);
            }
        }  else if (Node.Right->RetTy.Id == TypeId::EMPTYLIST) {
            Node.RetTy = Type(TypeId::LIST);
            Node.RetTy.Subtypes.push_back(Node.Left->RetTy);
            Node.Right->RetTy = Node.RetTy;
        } else {
            throw Error::Binary("Right must be a ListExpression",
                                   Node);
        }
    }

    void TypeChecker::visit(Par &Node) {
        // Visit children
        Node.Child->accept(*this);
        // Visit stops here

        // Code starts here
        Node.RetTy = Node.Child->RetTy;
        // Code stops here
    }

    void TypeChecker::visit(Not &Node) {
        // Visit children
        Node.Child->accept(*this);
        // Visit stops here

        if (Node.Child->RetTy.Id == TypeId::BOOL) {
            Node.RetTy = Node.Child->RetTy;
        } else {
            throw Error::Unary("Operator only operates on Bool typed children",
                                  Node);
        }
    }

    void TypeChecker::visit(IntPattern &Node) {

    }

    void TypeChecker::visit(FloatPattern &Node) {
    }

    void TypeChecker::visit(BoolPattern &Node) {
    }

    void TypeChecker::visit(CharPattern &Node) {
    }

    void TypeChecker::visit(ListPattern &Node) {
        // Visit children
        for (auto &pattern : Node.Patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        if (Node.Patterns.size() == 0) {
            Node.RetTy = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < Node.Patterns.size() - 1; ++i) {
                if (Node.Patterns[i]->RetTy != Node.Patterns[i + 1]->RetTy) {
                    throw Error::Expected("All items in a ListExpression must be of the same type",
                                             Node.Patterns[i]->str(),
                                             Node.Patterns[i + 1]->str(),
                                             Node.Loc);
                }
            }

            Node.RetTy = Type(TypeId::LIST);
            Node.RetTy.Subtypes.push_back(Node.Patterns[0]->RetTy);
        }
    }

    void TypeChecker::visit(TuplePattern &Node) {
        // Visit children
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }
        // Visit stops here

        Node.RetTy = Type(TypeId::TUPLE);

        for (auto &Pattern : Node.Patterns) {
            Node.RetTy.Subtypes.push_back(Pattern->RetTy);
        }
    }

    void TypeChecker::visit(ListSplit &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Right->RetTy.Id == TypeId::LIST) {
            if (Node.Left->RetTy == Node.Right->RetTy.Subtypes.front()) {
                Node.RetTy = Node.Right->RetTy;
            } else {
                throw Error::Expected("Left must be the same type as the right Lists children",
                                         Node.Right->RetTy.Subtypes[0].str(),
                                         Node.Left->RetTy.str(),
                                         Node.Loc);
            }
        } else if (Node.Right->RetTy.Id == TypeId::STRING) {
            if (Node.Left->RetTy.Id == TypeId::CHAR) {
                Node.RetTy = Node.Right->RetTy;
            } else {
                throw Error::Expected("Left must be type CharPattern, when right is String",
                                         "CharPattern",
                                         Node.Left->RetTy.str(),
                                         Node.Loc);
            }
        } else if (Node.Right->RetTy.Id == TypeId::EMPTYLIST) {
            Node.RetTy = Type(TypeId::LIST);
            Node.RetTy.Subtypes.push_back(Node.Left->RetTy);
        } else {
            throw Error::Expected("Right must be a ListExpression",
                                     "ListExpression",
                                     Node.Right->RetTy.str(),
                                     Node.Loc);
        }
    }

    void TypeChecker::visit(ListExpression &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here

        if (Node.Elements.size() == 0) {
            Node.RetTy = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < Node.Elements.size() - 1; ++i) {
                if (Node.Elements[i]->RetTy != Node.Elements[i + 1]->RetTy) {
                    throw Error::Expected("All items in a ListExpression must be same type",
                                             Node.Elements[i]->str(),
                                             Node.Elements[i + 1]->str(),
                                             Node.Elements[i + 1]->Loc);
                }
            }
            Node.RetTy = Type(TypeId::LIST);
            Node.RetTy.Subtypes.push_back(Node.Elements[0]->RetTy);
        }
    }

    void TypeChecker::visit(TupleExpression &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here

        Node.RetTy = Type(TypeId::TUPLE);

        for (size_t i = 0; i < Node.Elements.size(); ++i) {
            Node.RetTy.Subtypes.push_back(Node.Elements[i]->RetTy);
        }
    }


    void TypeChecker::visit(IdPattern &Node) {
        if (Node.Scp->exists(Node.Val)) {
            Node.RetTy = Node.Scp->getType(Node.Val);
        } else {
            throw Error(Node.Val + ": Id does not exist in the current scope",
                           Node.Loc);
        }
    }

    void TypeChecker::visit(Call &Node) {

        // Visit children
        Node.Callee->accept(*this);
        for (auto &Arg : Node.Args) {
            Arg->accept(*this);
        }
        // Visit stops here

        if (Node.Callee->RetTy.Id != TypeId::SIGNATURE) {
            throw Error::Expected("Can't call a type that is not a Signature",
                         "Signature",
                         Node.Callee->RetTy.str(),
                         Node.Callee->Loc);
        }

        if (Node.Args.size() + 1 != Node.Callee->RetTy.Subtypes.size()) {
            throw Error::Expected("Wrong number of arguments",
                                  to_string(Node.Callee->RetTy.Subtypes.size() - 1),
                                  to_string(Node.Args.size()),
                                  Node.Loc);
        }

        for (size_t i = 0; i < Node.Args.size(); ++i) {
            if (Node.Args[i]->RetTy.Id == TypeId::EMPTYLIST &&
                    Node.Callee->RetTy.Subtypes[i].Id == TypeId::LIST)
                Node.Args[i]->RetTy = Node.Callee->RetTy.Subtypes[i];
            else if (Node.Args[i]->RetTy != Node.Callee->RetTy.Subtypes[i]) {
                throw Error::Expected("Function was called with an invalid argument",
                                         Node.Callee->RetTy.Subtypes[i].str(),
                                         Node.Args[i]->RetTy.str(),
                                         Node.Args[i]->Loc);
            }
        }

        Node.RetTy = Node.Callee->RetTy.Subtypes.back();
    }
}
