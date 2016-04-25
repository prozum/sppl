#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"


using namespace std;
namespace semantics
{
    TypeChecker::TypeChecker() { }

    void TypeChecker::visit(Program &Node) {
        // Visit children
        for (auto &Decl : Node.Decls) {
            Decl->accept(*this);
        }
        // Visit stops here
    }

    void TypeChecker::visit(Function &Node) {
        CurFunc = &Node;

        auto StrList = Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }));

        if (Node.Id == "main") {
            if (Node.Signature.Subtypes.front() != StrList) {
                addError(Error::Expected("Declaration of \"main\" had wrong input type",
                         StrList.str(),
                         Node.Signature.Subtypes.front().str(),
                         Node.Loc));
            } else if (Node.Signature.Subtypes.size() != 2) {
                addError(Error::Expected("Function \"main\" had wrong number of input",
                         "2",
                         to_string(Node.Signature.Subtypes.size()),
                         Node.Loc));
            }
        }

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
    }

    void TypeChecker::visit(Case &Node) {
        // Visit children
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }

        try {
            Node.Expr->accept(*this);
        }
        catch (Error err) {
            Errors.push_back(err);
            return;
        }

        // Set signature for anonymous function
        if (CurFunc->Anon) {
            CurFunc->Signature = Type(TypeId::SIGNATURE, vector<Type>({Node.Expr->RetTy}));
        }

        // Make sure there is the same number of patterns as arguments to the function
        if (Node.Patterns.size() != CurFunc->Signature.Subtypes.size() - 1) {
            throw Error::Expected("Wrong pattern count",
                                  to_string(CurFunc->Signature.Subtypes.size() - 1),
                                  to_string(Node.Patterns.size()),
                                  Node.Loc);
        }

        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            if (Node.Patterns[i]->RetTy != CurFunc->Signature.Subtypes[i]) {
                throw Error::Expected("Wrong pattern type",
                                      CurFunc->Signature.Subtypes[i].str(),
                                      Node.Patterns[i]->RetTy.str(),
                                      Node.Patterns[i]->Loc);
            }
        }

        if (CurFunc->Signature.Subtypes.back() != Node.Expr->RetTy) {
            throw Error::Expected("Wrong return type",
                                  CurFunc->Signature.Subtypes.back().str(),
                                  Node.Expr->RetTy.str(),
                                  Node.Loc);
        }
    }

    void TypeChecker::visit(Or &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);

        if (Node.Left->RetTy.Id != TypeId::BOOL ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Bool typed children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(And &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Left->RetTy.Id != TypeId::BOOL ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Bool typed children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Equal &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(NotEqual &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Lesser &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Greater &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(LesserEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(GreaterEq &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Add &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Sub &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Mul &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Div &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Mod &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(ListAdd &Node) {
        // Visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // Visit stops here

        if (Node.Right->RetTy.Id != TypeId::LIST) {
            throw Error::Binary("Right must be a ListExpression",
                                Node);
        }

        if (Node.Left->RetTy != Node.Right->RetTy.Subtypes.front()) {
            throw Error::Binary("Left type must be same type of right ListExpression",
                                Node);
        }

        Node.RetTy = Node.Right->RetTy;
    }

    void TypeChecker::visit(ParExpr &Node) {
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

        if (Node.Child->RetTy.Id != TypeId::BOOL) {
            throw Error::Unary("Operator only operates on Bool typed children",
                               Node);
        }

        Node.RetTy = Node.Child->RetTy;
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

        for (size_t i = 0; i < Node.Patterns.size() - 1; ++i) {
            if (Node.Patterns[i]->RetTy != Node.Patterns[i + 1]->RetTy) {
                throw Error::Expected("All items in a ListExpression must be of the same type",
                                      Node.Patterns[i]->str(),
                                      Node.Patterns[i + 1]->str(),
                                      Node.Loc);
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

        if (Node.Right->RetTy.Id != TypeId::LIST) {
            throw Error::Expected("Right must be a ListExpression",
                                  "ListExpression",
                                  Node.Right->RetTy.str(),
                                  Node.Loc);
        }

        if (Node.Left->RetTy != Node.Right->RetTy.Subtypes.front()) {
            throw Error::Expected("Left must be the same type as the right Lists children",
                                     Node.Right->RetTy.Subtypes[0].str(),
                                     Node.Left->RetTy.str(),
                                     Node.Loc);
        }

        Node.RetTy = Node.Right->RetTy;
    }

    void TypeChecker::visit(ListExpr &Node) {
        // Visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // Visit stops here

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

    void TypeChecker::visit(TupleExpr &Node) {
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

    void TypeChecker::visit(IntExpr &Node) { }
    void TypeChecker::visit(FloatExpr &Node) { }
    void TypeChecker::visit(StringExpr &Node) { }
    void TypeChecker::visit(CharExpr &Node) { }


    void TypeChecker::visit(IdPattern &Node) {
        if (!Node.Scp->declExists(Node.Val)) {
            throw Error(Node.Val + ": Id does not exist in the current scope",
                        Node.Loc);
        }

        Node.RetTy = Node.Scp->getDeclType(Node.Val);
    }

    void TypeChecker::visit(IdExpr &Node) {
        if (!Node.Scp->declExists(Node.Val)) {
            throw Error(Node.Val + ": Id does not exist in the current scope",
                        Node.Loc);
        }

        Node.RetTy = Node.Scp->getDeclType(Node.Val);
    }

    void TypeChecker::visit(CallExpr &Node) {

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
            if (Node.Args[i]->RetTy != Node.Callee->RetTy.Subtypes[i]) {
                throw Error::Expected("Function was called with an invalid argument",
                                         Node.Callee->RetTy.Subtypes[i].str(),
                                         Node.Args[i]->RetTy.str(),
                                         Node.Args[i]->Loc);
            }
        }

        Node.RetTy = Node.Callee->RetTy.Subtypes.back();
    }
}
