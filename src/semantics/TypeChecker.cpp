#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"


using namespace std;
namespace semantics
{
    TypeChecker::TypeChecker(Scope *Scp) : CurScope(Scp), GlobalScope(Scp) { }

    void TypeChecker::visit(Program &Node) {
        // visit children
        for (auto &Decl : Node.Decls) {
            Decl->accept(*this);
        }
    }

    void TypeChecker::visit(Function &Node) {
        CurFunc = &Node;


        if (Node.Id == "main") {
            auto StrList = Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }));

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

        // visit children
        for (auto &Case : Node.Cases) {
            auto CaseScope = new Scope(CurScope);
            CurScope->Children.push_back(unique_ptr<Scope>(CaseScope));
            CurScope = CaseScope;

            try {
                Case->accept(*this);
            }
            catch (Error err) {
                Errors.push_back(err);
            }

            CurScope = CurScope->Parent;
        }
    }

    void TypeChecker::visit(AlgebraicDT &Node) {}
    void TypeChecker::visit(Product &Node) {}

    void TypeChecker::visit(Case &Node) {

        // visit children
        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            Node.Patterns[i]->RetTy = CurFunc->Signature.Subtypes[i];
            Node.Patterns[i]->accept(*this);
        }
        Node.Expr->accept(*this);

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


    // Patterns
    void TypeChecker::visit(IntPattern &Node) {
        if (Node.RetTy.Id != TypeId::INT) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  Type(TypeId::INT).str(),
                                  Node.Loc);
        }
    }

    void TypeChecker::visit(FloatPattern &Node) {
        if (Node.RetTy.Id != TypeId::FLOAT) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  Type(TypeId::FLOAT).str(),
                                  Node.Loc);
        }
    }

    void TypeChecker::visit(BoolPattern &Node) {
        if (Node.RetTy.Id != TypeId::BOOL) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  Type(TypeId::BOOL).str(),
                                  Node.Loc);
        }
    }

    void TypeChecker::visit(CharPattern &Node) {
        if (Node.RetTy.Id != TypeId::CHAR) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  Type(TypeId::CHAR).str(),
                                  Node.Loc);
        }
    }

    void TypeChecker::visit(StringPattern &Node) {
        if (Node.RetTy.Id != TypeId::STRING) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  Type(TypeId::STRING).str(),
                                  Node.Loc);
        }
    }

    void TypeChecker::visit(ListPattern &Node) {
        if (Node.RetTy.Id != TypeId::LIST) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  "List",
                                  Node.Loc);
        }

        // visit children
        for (auto &Pattern : Node.Patterns) {
            Pattern->RetTy = Node.RetTy.Subtypes.front();
            Pattern->accept(*this);
        }
    }

    void TypeChecker::visit(TuplePattern &Node) {
        if (Node.RetTy.Id != TypeId::LIST) {
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  "Tuple",
                                  Node.Loc);
        }

        if (Node.RetTy.Subtypes.size() != Node.Patterns.size()) {
            throw Error::Expected("Tuple is not of correct size",
                                  to_string(Node.RetTy.Subtypes.size()),
                                  to_string(Node.Patterns.size()),
                                  Node.Loc);
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
            throw Error::Expected("Type mismatch",
                                  Node.RetTy.str(),
                                  "List",
                                  Node.Loc);
        }


        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
    }


    void TypeChecker::visit(IdPattern &Node) {
        Type Out;

        if (CurScope->tryGetDecl(Node.Val, Out)) {
            throw Error(Node.Val + " was already declared in the current scope.",
                        Node.Loc);
        }

        CurScope->Decls.insert({ Node.Val, Node.RetTy });
    }

    void TypeChecker::visit(WildPattern &Node) { }

    void TypeChecker::visit(AlgebraicPattern &Node) {
        auto Got = CurScope->Constructors.find(Node.Constructor);

        if (Got != CurScope->Constructors.end()) {
            if (Got->second.Values.size() != Node.Patterns.size()) {
                throw Error::Expected("Data constructor for " + Node.Constructor + " has the wrong number of input",
                                      to_string(Got->second.Values.size()),
                                      to_string(Node.Patterns.size()),
                                      Node.Loc);
            }

            for (size_t i = 0; i < Node.Patterns.size(); ++i) {
                Node.Patterns[i]->RetTy = Got->second.Values[i];
                Node.Patterns[i]->accept(*this);
            }
        } else {
            throw Error(Node.Constructor + " doesn't exist in the current scope",
                        Node.Loc);
        }
    }

    void TypeChecker::visit(ParPattern &Node) {
        Node.Pat->RetTy = Node.RetTy;
        Node.Pat->accept(*this);
    }


    // Expressions
    void TypeChecker::visit(Or &Node) {
        // visit children
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
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if (Node.Left->RetTy.Id != TypeId::BOOL ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Bool typed children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Equal &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Type(TypeId::BOOL);
    }

    void TypeChecker::visit(NotEqual &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or FloatPattern children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Lesser &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Greater &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(LesserEq &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(GreaterEq &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Add &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Sub &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Mul &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Div &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(Mod &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

        if ((Node.Left->RetTy.Id != TypeId::INT && Node.Left->RetTy.Id != TypeId::FLOAT) ||
            Node.Left->RetTy.Id != Node.Right->RetTy.Id) {
            throw Error::Binary("Operator only operates on Int or Float children",
                                Node);
        }

        Node.RetTy = Node.Left->RetTy;
    }

    void TypeChecker::visit(ListAdd &Node) {
        // visit children
        Node.Left->accept(*this);
        Node.Right->accept(*this);
        // visit stops here

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

    void TypeChecker::visit(To &Node) {
        // TODO Implement To Type Checking
    }

    void TypeChecker::visit(AlgebraicExpr &Node) {
        Product *Out;

        for (auto &Expr: Node.Exprs) {
            Expr->accept(*this);
        }

        if (CurScope->tryGetCon(Node.Constructor, Out)) {




        } else {
            throw Error(Node.Constructor + ": Data constructor does not exist in the current scope",
                        Node.Loc);
        }
    }

    void TypeChecker::visit(LambdaFunction &Node) {
        // TODO Implement LambdaFunction Type Checking
    }
    void TypeChecker::visit(LambdaArg &Node) {
        // TODO Implement LambdaArg Type Checking
    }
    void TypeChecker::visit(ProducerConsumer &Node) {
        // TODO Implement ProducerConsumer Type Checking
    }
    void TypeChecker::visit(Concat &Node) {
        // TODO Implement Concat Type Checking
    }

    void TypeChecker::visit(ParExpr &Node) {
        // visit children
        Node.Child->accept(*this);

        Node.RetTy = Node.Child->RetTy;
    }
    void TypeChecker::visit(Negative &Node) {
        // visit children
        Node.Child->accept(*this);

        if (Node.Child->RetTy.Id != TypeId::INT && Node.Child->RetTy.Id != TypeId::FLOAT) {
            throw Error::Unary("Operator only operates on Int or Float typed children",
                               Node);
        }

        Node.RetTy = Node.Child->RetTy;
    }

    void TypeChecker::visit(Not &Node) {
        // visit children
        Node.Child->accept(*this);

        if (Node.Child->RetTy.Id != TypeId::BOOL) {
            throw Error::Unary("Operator only operates on Bool typed children",
                               Node);
        }

        Node.RetTy = Node.Child->RetTy;
    }

    void TypeChecker::visit(ListExpr &Node) {
        // visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }

        if (Node.Elements.size() != 0) {
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
        } else {
            Node.RetTy = Type(TypeId::LIST);
            Node.RetTy.Subtypes.push_back(Type(TypeId::GENERIC));
        }

    }

    void TypeChecker::visit(TupleExpr &Node) {
        // visit children
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
        }
        // visit stops here

        Node.RetTy = Type(TypeId::TUPLE);

        for (size_t i = 0; i < Node.Elements.size(); ++i) {
            Node.RetTy.Subtypes.push_back(Node.Elements[i]->RetTy);
        }
    }

    void TypeChecker::visit(IdExpr &Node) {
        Type Out;

        if (!CurScope->tryGetDecl(Node.Val, Out)) {
            throw Error(Node.Val + ": Id does not exist in the current scope",
                        Node.Loc);
        }

        Node.RetTy = Out;
    }

    void TypeChecker::visit(CallExpr &Node) {

        // visit children
        Node.Callee->accept(*this);
        for (auto &Arg : Node.Args) {
            Arg->accept(*this);
        }
        // visit stops here

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
            if (false) {
                // TODO Generic support here
            } else if (Node.Args[i]->RetTy != Node.Callee->RetTy.Subtypes[i]) {
                throw Error::Expected("Function was called with an invalid argument",
                                      Node.Callee->RetTy.Subtypes[i].str(),
                                      Node.Args[i]->RetTy.str(),
                                      Node.Args[i]->Loc);
            }
        }

        Node.RetTy = Node.Callee->RetTy.Subtypes.back();
    }

    void TypeChecker::visit(IntExpr &Node) { }
    void TypeChecker::visit(FloatExpr &Node) { }
    void TypeChecker::visit(StringExpr &Node) { }
    void TypeChecker::visit(CharExpr &Node) { }
    void TypeChecker::visit(BoolExpr &Node) { }

}
