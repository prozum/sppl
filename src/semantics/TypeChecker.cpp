#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"

using namespace std;
using namespace common;
using namespace semantics;

TypeChecker::TypeChecker(Scope *Scp) : CurScope(Scp), GlobalScope(Scp) {}

void TypeChecker::visit(Program &Node) {
    // Visit children
    for (auto &Decl : Node.Decls) {
        Decl->accept(*this);
        setSafe();
    }
}

// Declarations
void TypeChecker::visit(Function &Node) {
    CurFunc = &Node;

    if (Node.Id == "main") {
        auto StrList = Type(TypeId::LIST, vector<Type>({Type(TypeId::STRING)}));

        if (Node.Signature.Subtypes.front() != StrList) {
            addError(Error::Expected(
                "Declaration of \"main\" had wrong input type", StrList.str(),
                Node.Signature.Subtypes.front().str(), Node.Loc));
            return;
        } else if (Node.Signature.Subtypes.size() != 2) {
            addError(Error::Expected(
                "Function \"main\" had wrong number of input", "2",
                to_string(Node.Signature.Subtypes.size()), Node.Loc));
            return;
        }
    }

    // Visit children
    for (auto &Case : Node.Cases) {
        auto CaseScope = new Scope(CurScope);
        CurScope->Children.push_back(unique_ptr<Scope>(CaseScope));
        CurScope = CaseScope;

        Case->accept(*this);
        setSafe();

        CurScope = CurScope->Parent;
    }
}

void TypeChecker::visit(AlgebraicDT &Node) {}
void TypeChecker::visit(Product &Node) {}

void TypeChecker::visit(Case &Node) {

    // Visit children
    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        Node.Patterns[i]->RetTy = CurFunc->Signature.Subtypes[i];
        Node.Patterns[i]->accept(*this);
    }

    if (Node.When) {
        Node.When->accept(*this);
    }
    Node.Expr->accept(*this);
    if (checkNotSafe())
        return;

    // Set signature for anonymous function
    if (CurFunc->Anon) {
        CurFunc->Signature =
            Type(TypeId::SIGNATURE, vector<Type>({Node.Expr->RetTy}));
    }

    // Make sure there is the same number of patterns as arguments to the
    // function
    if (Node.Patterns.size() != CurFunc->Signature.Subtypes.size() - 1) {
        addError(Error::Expected("Wrong pattern count",
                              to_string(CurFunc->Signature.Subtypes.size() - 1),
                              to_string(Node.Patterns.size()), Node.Loc));
        return;
    }

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        if (Node.Patterns[i]->RetTy != CurFunc->Signature.Subtypes[i]) {
            addError(Error::Expected(
                "Wrong pattern type", CurFunc->Signature.Subtypes[i].str(),
                Node.Patterns[i]->RetTy.str(), Node.Patterns[i]->Loc));
            return;
        }
    }

    if (Node.When) {
        if (Node.When->RetTy.Id != TypeId::BOOL) {
            addError(Error::Expected("Type mismatch in the when expression",
                                  Type(TypeId::BOOL).str(),
                                  Node.When->RetTy.str(), Node.When->Loc));
            return;
        }
    }

    if (containsEmptyList(Node.Expr->RetTy)) {
        resolveEmptyList(Node.Expr->RetTy, CurFunc->Signature.Subtypes.back());
    }

    if (CurFunc->Signature.Subtypes.back() != Node.Expr->RetTy &&
        CurFunc->Signature.Subtypes.back() != TypeId::VOID) {
        addError(Error::Expected("Wrong return type",
                              CurFunc->Signature.Subtypes.back().str(),
                              Node.Expr->RetTy.str(), Node.Expr->Loc));
        return;
    }
}

// Patterns
void TypeChecker::visit(IntPattern &Node) {
    if (Node.RetTy.Id != TypeId::INT) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(),
                              Type(TypeId::INT).str(), Node.Loc));
        return;
    }
}

void TypeChecker::visit(FloatPattern &Node) {
    if (Node.RetTy.Id != TypeId::FLOAT) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(),
                              Type(TypeId::FLOAT).str(), Node.Loc));
        return;
    }
}

void TypeChecker::visit(BoolPattern &Node) {
    if (Node.RetTy.Id != TypeId::BOOL) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(),
                              Type(TypeId::BOOL).str(), Node.Loc));
        return;
    }
}

void TypeChecker::visit(CharPattern &Node) {
    if (Node.RetTy.Id != TypeId::CHAR) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(),
                              Type(TypeId::CHAR).str(), Node.Loc));
        return;
    }
}

void TypeChecker::visit(IdPattern &Node) {
    Type Out;

    if (CurScope->tryGetDecl(Node.Val, Out)) {
        addError(Error(Node.Val + " was already declared in the current scope.",
                    Node.Loc));
        return;
    }

    CurScope->Decls.insert({Node.Val, Node.RetTy});
}

void TypeChecker::visit(WildPattern &Node) {
    // Nothing should be checked in the wild pattern
}

void TypeChecker::visit(StringPattern &Node) {
    if (Node.RetTy.Id != TypeId::STRING) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(),
                              Type(TypeId::STRING).str(), Node.Loc));
        return;
    }
}

void TypeChecker::visit(ListPattern &Node) {
    if (Node.RetTy.Id != TypeId::LIST) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(), "List",
                              Node.Loc));
        return;
    }

    // visit children
    for (auto &Pattern : Node.Patterns) {
        Pattern->RetTy = Node.RetTy.Subtypes.front();
        Pattern->accept(*this);
    }
}

void TypeChecker::visit(TuplePattern &Node) {
    if (Node.RetTy.Id != TypeId::TUPLE) {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(), "Tuple",
                              Node.Loc));
        return;
    }

    if (Node.RetTy.Subtypes.size() != Node.Patterns.size()) {
        addError(Error::Expected("Tuple is not of correct size",
                              to_string(Node.RetTy.Subtypes.size()),
                              to_string(Node.Patterns.size()), Node.Loc));
        return;
    }

    // visit children
    for (size_t i = 0; i < Node.RetTy.Subtypes.size(); ++i) {
        Node.Patterns[i]->RetTy = Node.RetTy.Subtypes[i];
        Node.Patterns[i]->accept(*this);
    }
}

void TypeChecker::visit(ListSplit &Node) {
    if (Node.RetTy.Id == TypeId::LIST) {
        Node.Left->RetTy = Node.RetTy.Subtypes.front();
        Node.Right->RetTy = Node.RetTy;
    } else if (Node.RetTy.Id == TypeId::STRING) {
        Node.Left->RetTy = Type(TypeId::CHAR);
        Node.Right->RetTy = Node.RetTy;
    } else {
        addError(Error::Expected("Type mismatch", Node.RetTy.str(), "List",
                              Node.Loc));
        return;
    }

    // visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
}

void TypeChecker::visit(AlgebraicPattern &Node) {
    auto Got = CurScope->Constructors.find(Node.Constructor);

    if (Got != CurScope->Constructors.end()) {
        if (Got->second.Values.size() != Node.Patterns.size()) {
            addError(Error::Expected("Data constructor for " + Node.Constructor +
                                      " has the wrong number of input",
                                  to_string(Got->second.Values.size()),
                                  to_string(Node.Patterns.size()), Node.Loc));
            return;
        }

        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            Node.Patterns[i]->RetTy = Got->second.Values[i];
            Node.Patterns[i]->accept(*this);
        }
    } else {
        addError(Error(Node.Constructor + " doesn't exist in the current scope",
                    Node.Loc));
        return;
    }
}

void TypeChecker::visit(ParPattern &Node) {
    Node.Pat->RetTy = Node.RetTy;
    Node.Pat->accept(*this);
}

// Expressions
void TypeChecker::visit(Or &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if (Node.Left->RetTy.Id != TypeId::BOOL ||
        Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary("Operator only operates on Bool typed children",
                            Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(And &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if (Node.Left->RetTy.Id != TypeId::BOOL ||
        Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary("Operator only operates on Bool typed children",
                            Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(Equal &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if (Node.Left->RetTy != Node.Right->RetTy) {
        addError(Error::Binary(
                "Operator only operates on children of the same type", Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(NotEqual &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if (Node.Left->RetTy != Node.Right->RetTy) {
        addError(Error::Binary(
            "Operator only operates on children of the same type", Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(Lesser &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(Greater &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(LesserEq &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(GreaterEq &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy.Id = TypeId::BOOL;
}

void TypeChecker::visit(Add &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy = Node.Left->RetTy;
}

void TypeChecker::visit(Sub &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy = Node.Left->RetTy;
}

void TypeChecker::visit(Mul &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy = Node.Left->RetTy;
}

void TypeChecker::visit(Div &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy = Node.Left->RetTy;
}

void TypeChecker::visit(Mod &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if ((Node.Left->RetTy.Id != TypeId::INT &&
         Node.Left->RetTy.Id != TypeId::FLOAT) ||
         Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
        addError(Error::Binary(
            "Operator only operates on Int or Float children", Node));
        return;
    }

    Node.RetTy = Node.Left->RetTy;
}

void TypeChecker::visit(ListAdd &Node) {
    // Visit children
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if (containsEmptyList(Node.Right->RetTy)) {
        resolveEmptyList(Node.Right->RetTy, CurFunc->Signature.Subtypes.back());
    }

    if (Node.Right->RetTy.Id == TypeId::LIST) {
        if (Node.Left->RetTy != Node.Right->RetTy.Subtypes.front()) {
            addError(Error::Binary(
                    "Left type must be same type of right List", Node));
            return;
        }
    } else if (Node.Right->RetTy.Id == TypeId::STRING) {
        if (Node.Left->RetTy.Id != TypeId::CHAR) {
            addError(Error::Binary(
                    "Left type must be a Char, when adding to a String", Node));
            return;
        }
    } else {
        addError(Error::Binary("Right must be a List", Node));
        return;
    }


    Node.RetTy = Node.Right->RetTy;
}

void TypeChecker::visit(To &Node) {
    // TODO Implement To Type Checking
    addError(Error::NotImplemented(Node.Loc));
}

void TypeChecker::visit(AlgebraicExpr &Node) {
    Product *Out;

    for (auto &Expr : Node.Exprs) {
        Expr->accept(*this);
    }

    if (CurScope->tryGetCon(Node.Constructor, Out)) {
        // TODO Implement Checking if constructor was found
        addError(Error::NotImplemented(Node.Loc));
    } else {
        addError(Error(
            Node.Constructor +
                ": Data constructor does not exist in the current scope",
            Node.Loc));
        return;
    }
}

void TypeChecker::visit(LambdaFunction &Node) {
    // TODO Implement LambdaFunction Type Checking
    addError(Error::NotImplemented(Node.Loc));
}
void TypeChecker::visit(LambdaArg &Node) {
    // TODO Implement LambdaArg Type Checking
    addError(Error::NotImplemented(Node.Loc));
}
void TypeChecker::visit(ProducerConsumer &Node) {
    // TODO Implement ProducerConsumer Type Checking
    addError(Error::NotImplemented(Node.Loc));
}
void TypeChecker::visit(Concat &Node) {
    Node.Left->accept(*this);
    Node.Right->accept(*this);
    if (checkNotSafe())
        return;

    if (containsEmptyList(Node.Left->RetTy)) {
        resolveEmptyList(Node.Left->RetTy, Node.Right->RetTy);
    }

    if (containsEmptyList(Node.Right->RetTy)) {
        resolveEmptyList(Node.Right->RetTy, Node.Left->RetTy);
    }

    if (Node.Left->RetTy != Node.Right->RetTy) {
        addError(Error::Binary("Concat can only operate on children of the same type", Node));
        return;
    }

    if (Node.Right->RetTy.Id == TypeId::EMPTYLIST) {
        addError(Error::Binary("Concat cannot operate on two lists of unknown type", Node));
        return;
    }

    if (Node.Right->RetTy.Id != TypeId::LIST && Node.Right->RetTy.Id != TypeId::STRING) {
        addError(Error::Binary("Concat can only operate on lists", Node));
        return;
    }

    Node.RetTy = Node.Left->RetTy;
}

void TypeChecker::visit(ParExpr &Node) {
    // Visit children
    Node.Child->accept(*this);

    Node.RetTy = Node.Child->RetTy;
}
void TypeChecker::visit(Negative &Node) {
    // visit children
    Node.Child->accept(*this);
    if (checkNotSafe())
        return;

    if (Node.Child->RetTy.Id != TypeId::INT &&
        Node.Child->RetTy.Id != TypeId::FLOAT) {
        addError(Error::Unary(
            "Operator only operates on Int or Float typed children", Node));
        return;
    }

    Node.RetTy = Node.Child->RetTy;
}

void TypeChecker::visit(Not &Node) {
    // Visit children
    Node.Child->accept(*this);
    if (checkNotSafe())
        return;

    if (Node.Child->RetTy.Id != TypeId::BOOL) {
        addError(Error::Unary("Operator only operates on Bool typed children",
                           Node));
        return;
    }

    Node.RetTy = Node.Child->RetTy;
}

void TypeChecker::visit(ListExpr &Node) {
    // Visit children
    for (auto &Element : Node.Elements) {
        Element->accept(*this);
    }

    if (Node.Elements.size() != 0) {
        auto ListType = Node.Elements.front()->RetTy;
        for (size_t i = 0; i < Node.Elements.size() - 1; ++i) {
            if (ListType != Node.Elements[i + 1]->RetTy) {
                addError(Error::Expected(
                    "All items in a List must be same type",
                    ListType.str(), Node.Elements[i + 1]->RetTy.str(),
                    Node.Elements[i + 1]->Loc));
                return;
            }
        }

        Node.RetTy.Subtypes.push_back(Node.Elements[0]->RetTy);
    } else {
        Node.RetTy = Type(TypeId::EMPTYLIST);
    }
}

void TypeChecker::visit(TupleExpr &Node) {
    // Visit children
    for (auto &Element : Node.Elements) {
        Element->accept(*this);
    }

    Node.RetTy = Type(TypeId::TUPLE);

    for (size_t i = 0; i < Node.Elements.size(); ++i) {
        Node.RetTy.Subtypes.push_back(Node.Elements[i]->RetTy);
    }
}

void TypeChecker::visit(IdExpr &Node) {
    Type Out;

    if (!CurScope->tryGetDecl(Node.Val, Out)) {
        addError(Error(Node.Val + ": Id does not exist in the current scope",
                    Node.Loc));
        return;
    }

    Node.RetTy = Out;
}

void TypeChecker::visit(CallExpr &Node) {

    // Visit children
    Node.Callee->accept(*this);
    for (auto &Arg : Node.Args) {
        Arg->accept(*this);
    }

    if (Node.Callee->RetTy.Id != TypeId::SIGNATURE) {
        addError(Error::Expected("Can't call a type that is not a Signature",
                              "Signature", Node.Callee->RetTy.str(),
                              Node.Callee->Loc));
        return;
    }

    if (Node.Args.size() + 1 != Node.Callee->RetTy.Subtypes.size()) {
        addError(Error::Expected("Wrong number of arguments",
                              to_string(Node.Callee->RetTy.Subtypes.size() - 1),
                              to_string(Node.Args.size()), Node.Loc));
        return;
    }

    for (size_t i = 0; i < Node.Args.size(); ++i) {
        if (containsEmptyList(Node.Args[i]->RetTy)) {
            resolveEmptyList(Node.Args[i]->RetTy, Node.Callee->RetTy.Subtypes[i]);
        }

        if (Node.Args[i]->RetTy != Node.Callee->RetTy.Subtypes[i]) {
            addError(Error::Expected(
                "Function was called with an invalid argument",
                Node.Callee->RetTy.Subtypes[i].str(), Node.Args[i]->RetTy.str(),
                Node.Args[i]->Loc));
            return;
        }
    }

    Node.RetTy = Node.Callee->RetTy.Subtypes.back();
}

void TypeChecker::visit(IntExpr &Node) { }
void TypeChecker::visit(FloatExpr &Node) { }
void TypeChecker::visit(StringExpr &Node) { }
void TypeChecker::visit(CharExpr &Node) { }
void TypeChecker::visit(BoolExpr &Node) { }

bool TypeChecker::containsEmptyList(Type &Ty) {
    switch (Ty.Id) {
        case TypeId::EMPTYLIST:
            return true;
        case TypeId::LIST:
        case TypeId::TUPLE:
            for (auto &Tpe: Ty.Subtypes) {
                if (containsEmptyList(Tpe))
                    return true;
            }
        default:
            return false;
    }
}

// TODO Can't resolve empty list in tuple
void TypeChecker::resolveEmptyList(Type &Ty, Type &Resolver) {
    switch (Ty.Id) {
        case TypeId::EMPTYLIST:
            if (Resolver.Id == TypeId::LIST || Resolver.Id == TypeId::STRING) {
                Ty = Resolver;
            }
            break;
        case TypeId::LIST:
        case TypeId::TUPLE:
            for (size_t i = 0; i < Ty.Subtypes.size() && i < Resolver.Subtypes.size(); ++i) {
                resolveEmptyList(Ty.Subtypes[i], Resolver.Subtypes[i]);
            }
            break;
        default:
            break;
    }
}
