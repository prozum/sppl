#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"


using namespace std;
namespace semantics
{
    TypeChecker::TypeChecker() { }

    void TypeChecker::visit(Program &Node) {
        auto StrList = Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }));

        // Visit children
        for (auto &Func : Node.Funcs) {
            Func->accept(*this);

            if (Func->Id == "main") {
                if (Func->Signature.Subtypes.front() != StrList) {
                    addError(Error::Expected("Declaration of \"main\" had wrong input type",
                                          StrList.str(),
                                          Func->Signature.Subtypes.front().str(),
                                          Func->Loc));
                } else if (Func->Signature.Subtypes.size() != 2) {
                    addError(Error::Expected("Function \"main\" had wrong number of input",
                                          "2",
                                          to_string(Func->Signature.Subtypes.size()),
                                          Func->Loc));
                }
            }

        }
        // Visit stops here
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
            Node.Ty = Node.Signature.Subtypes.back();
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
            CurFunc->Signature = Type(TypeId::SIGNATURE, vector<Type>({Node.Expr->Ty}));
        }

        if (Node.Patterns.size() != CurFunc->Signature.Subtypes.size() - 1) {
            throw Error::Expected("Wrong pattern count",
                                  to_string(CurFunc->Signature.Subtypes.size() - 1),
                                  to_string(Node.Patterns.size()),
                                  Node.Loc);
        }

        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            if (Node.Patterns[i]->Ty.Id == TypeId::EMPTYLIST &&
                    CurFunc->Signature.Subtypes[i].Id == TypeId::LIST) {
                Node.Patterns[i]->Ty = CurFunc->Signature.Subtypes[i];
            }

            if (Node.Patterns[i]->Ty != CurFunc->Signature.Subtypes[i]) {
                throw Error::Expected("Wrong pattern type",
                                         CurFunc->Signature.Subtypes[i].str(),
                                         Node.Patterns[i]->Ty.str(),
                                         Node.Patterns[i]->Loc);
            }
        }

        if (Node.Expr->Ty.Id == TypeId::EMPTYLIST &&
                CurFunc->Signature.Subtypes.back().Id == TypeId::LIST)
            Node.Expr->Ty = CurFunc->Signature.Subtypes.back();
        else if (CurFunc->Signature.Subtypes.back() != Node.Expr->Ty) {
            throw Error::Expected("Wrong return type",
                                     CurFunc->Ty.str(),
                                     Node.Expr->Ty.str(),
                                     Node.Loc);
        }
    }

    void TypeChecker::visit(Or &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->Ty.Id == TypeId::BOOL && Node.Right->Ty.Id == TypeId::BOOL) {
            Node.Ty = Node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   Node);
        }
    }

    void TypeChecker::visit(And &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->Ty == TypeId::BOOL && Node.Right->Ty == TypeId::BOOL) {
            Node.Ty = Node.Left->Ty;
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

        if (Node.Left->Ty == Node.Right->Ty) {
            Node.Ty = Type(TypeId::BOOL);
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

        if (Node.Left->Ty == Node.Right->Ty) {
            Node.Ty = Type(TypeId::BOOL);
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

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Type(TypeId::BOOL);
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

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(LesserEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(GreaterEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(Add &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(Sub &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(Mul &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(Div &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(Mod &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->Ty.Id == TypeId::INT && Node.Right->Ty.Id == TypeId::INT) ||
            (Node.Left->Ty.Id == TypeId::FLOAT && Node.Right->Ty.Id == TypeId::FLOAT)) {
            Node.Ty = Node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   Node);
        }
    }

    void TypeChecker::visit(ListAdd &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Right->Ty.Id == TypeId::LIST) {
            if (Node.Left->Ty == Node.Right->Ty.Subtypes.front()) {
                Node.Ty = Node.Right->Ty;
            } else {
                throw Error::Binary("Left type must be same type of right List",
                                       Node);
            }
        } else if (Node.Right->Ty.Id == TypeId::STRING) {
            if (Node.Left->Ty.Id == TypeId::CHAR) {
                Node.Ty = Node.Right->Ty;
            } else {
                throw Error::Binary("Left type must be Char when right is String",
                                       Node);
            }
        }  else if (Node.Right->Ty.Id == TypeId::EMPTYLIST) {
            Node.Ty = Type(TypeId::LIST);
            Node.Ty.Subtypes.push_back(Node.Left->Ty);
            Node.Right->Ty = Node.Ty;
        } else {
            throw Error::Binary("Right must be a List",
                                   Node);
        }
    }

    void TypeChecker::visit(Par &Node) {
        // Visit children
        Node.Child->accept(*this);
        // Visit stops here

        // Code starts here
        Node.Ty = Node.Child->Ty;
        // Code stops here
    }

    void TypeChecker::visit(Not &Node) {
        // Visit children
        Node.Child->accept(*this);
        // Visit stops here

        if (Node.Child->Ty.Id == TypeId::BOOL) {
            Node.Ty = Node.Child->Ty;
        } else {
            throw Error::Unary("Operator only operates on Bool typed children",
                                  Node);
        }
    }

    void TypeChecker::visit(Int &Node) {

    }

    void TypeChecker::visit(Float &Node) {
    }

    void TypeChecker::visit(Bool &Node) {
    }

    void TypeChecker::visit(Char &Node) {
    }

    void TypeChecker::visit(String &Node) {
    }

    void TypeChecker::visit(ListPattern &Node) {
        // Visit children
        for (auto &pattern : Node.Patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        if (Node.Patterns.size() == 0) {
            Node.Ty = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < Node.Patterns.size() - 1; ++i) {
                if (Node.Patterns[i]->Ty != Node.Patterns[i + 1]->Ty) {
                    throw Error::Expected("All items in a List must be of the same type",
                                             Node.Patterns[i]->str(),
                                             Node.Patterns[i + 1]->str(),
                                             Node.Loc);
                }
            }

            Node.Ty = Type(TypeId::LIST);
            Node.Ty.Subtypes.push_back(Node.Patterns[0]->Ty);
        }
    }

    void TypeChecker::visit(TuplePattern &Node) {
        // Visit children
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }
        // Visit stops here

        Node.Ty = Type(TypeId::TUPLE);

        for (auto &Pattern : Node.Patterns) {
            Node.Ty.Subtypes.push_back(Pattern->Ty);
        }
    }

    void TypeChecker::visit(ListSplit &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Right->Ty.Id == TypeId::LIST) {
            if (Node.Left->Ty == Node.Right->Ty.Subtypes.front()) {
                Node.Ty = Node.Right->Ty;
            } else {
                throw Error::Expected("Left must be the same type as the right Lists children",
                                         Node.Right->Ty.Subtypes[0].str(),
                                         Node.Left->Ty.str(),
                                         Node.Loc);
            }
        } else if (Node.Right->Ty.Id == TypeId::STRING) {
            if (Node.Left->Ty.Id == TypeId::CHAR) {
                Node.Ty = Node.Right->Ty;
            } else {
                throw Error::Expected("Left must be type Char, when right is String",
                                         "Char",
                                         Node.Left->Ty.str(),
                                         Node.Loc);
            }
        } else if (Node.Right->Ty.Id == TypeId::EMPTYLIST) {
            Node.Ty = Type(TypeId::LIST);
            Node.Ty.Subtypes.push_back(Node.Left->Ty);
        } else {
            throw Error::Expected("Right must be a List",
                                     "List",
                                     Node.Right->Ty.str(),
                                     Node.Loc);
        }
    }

    void TypeChecker::visit(List &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here

        if (Node.Elements.size() == 0) {
            Node.Ty = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < Node.Elements.size() - 1; ++i) {
                if (Node.Elements[i]->Ty != Node.Elements[i + 1]->Ty) {
                    throw Error::Expected("All items in a List must be same type",
                                             Node.Elements[i]->str(),
                                             Node.Elements[i + 1]->str(),
                                             Node.Elements[i + 1]->Loc);
                }
            }
            Node.Ty = Type(TypeId::LIST);
            Node.Ty.Subtypes.push_back(Node.Elements[0]->Ty);
        }
    }

    void TypeChecker::visit(Tuple &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here

        Node.Ty = Type(TypeId::TUPLE);

        for (size_t i = 0; i < Node.Elements.size(); ++i) {
            Node.Ty.Subtypes.push_back(Node.Elements[i]->Ty);
        }
    }


    void TypeChecker::visit(Id &Node) {
        if (Node.Scp->exists(Node.Val)) {
            Node.Ty = Node.Scp->getType(Node.Val);
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

        if (Node.Callee->Ty.Id != TypeId::SIGNATURE) {
            throw Error::Expected("Can't call a type that is not a Signature",
                         "Signature",
                         Node.Callee->Ty.str(),
                         Node.Callee->Loc);
        }

        if (Node.Args.size() + 1 != Node.Callee->Ty.Subtypes.size()) {
            throw Error::Expected("Wrong number of arguments",
                                  to_string(Node.Callee->Ty.Subtypes.size() - 1),
                                  to_string(Node.Args.size()),
                                  Node.Loc);
        }

        for (size_t i = 0; i < Node.Args.size(); ++i) {
            if (Node.Args[i]->Ty.Id == TypeId::EMPTYLIST &&
                    Node.Callee->Ty.Subtypes[i].Id == TypeId::LIST)
                Node.Args[i]->Ty = Node.Callee->Ty.Subtypes[i];
            else if (Node.Args[i]->Ty != Node.Callee->Ty.Subtypes[i]) {
                throw Error::Expected("Function was called with an invalid argument",
                                         Node.Callee->Ty.Subtypes[i].str(),
                                         Node.Args[i]->Ty.str(),
                                         Node.Args[i]->Loc);
            }
        }

        Node.Ty = Node.Callee->Ty.Subtypes.back();
    }
}
