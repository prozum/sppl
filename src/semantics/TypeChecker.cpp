#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"


using namespace std;
namespace semantics
{
    TypeChecker::TypeChecker() { }

    void TypeChecker::visit(Program &node) {
        auto strlist = Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING) }));

        // Visit children
        for (auto &func: node.Funcs) {
            func->accept(*this);

            try {
                if (func->Id == "main") {
                    if (func->Signature.Subtypes.front() != strlist) {
                        throw Error::Expected("Declaration of \"main\" had wrong input type",
                                              strlist.str(),
                                              func->Signature.Subtypes.front().str(),
                                              func->Loc);
                    } else if (func->Signature.Subtypes.size() != 2) {
                        throw Error::Expected("Function \"main\" had wrong number of input",
                                              "2",
                                              to_string(func->Signature.Subtypes.size()),
                                              func->Loc);
                    }
                }
            }
            catch (Error err) {
                Errors.push_back(err);
            }

        }
        // Visit stops here
    }

    void TypeChecker::visit(Function &node) {
        CurFunc = &node;

        // Visit children
        for (auto &cse: node.Cases) {
            try {
                cse->accept(*this);
            }
            catch (Error err) {
                Errors.push_back(err);
            }
        }
        // Visit stops here

        // Set return type
        if (!hasError())
            node.Ty = node.Signature.Subtypes.back();
    }

    void TypeChecker::visit(Case &node) {
        // Visit children
        for (auto &pattern: node.Patterns) {
            pattern->accept(*this);
        }
        node.Expr->accept(*this);
        // Visit stops here

        // Set signature for anonymous function
        if (CurFunc->Anon) {
            CurFunc->Signature = Type(TypeId::SIGNATURE, vector<Type>({node.Expr->Ty}));
        }

        if (node.Patterns.size() != CurFunc->Signature.Subtypes.size() - 1) {
            throw Error::Expected("Wrong pattern count",
                                  to_string(CurFunc->Signature.Subtypes.size() - 1),
                                  to_string(node.Patterns.size()),
                                  node.Loc);
        }

        for (size_t i = 0; i < node.Patterns.size(); ++i) {
            if (node.Patterns[i]->Ty.Id == TypeId::EMPTYLIST &&
                    CurFunc->Signature.Subtypes[i].Id == TypeId::LIST) {
                node.Patterns[i]->Ty = CurFunc->Signature.Subtypes[i];
            }

            if (node.Patterns[i]->Ty != CurFunc->Signature.Subtypes[i]) {
                throw Error::Expected("Wrong pattern type",
                                         CurFunc->Signature.Subtypes[i].str(),
                                         node.Patterns[i]->Ty.str(),
                                         node.Patterns[i]->Loc);
            }
        }

        if (node.Expr->Ty.Id == TypeId::EMPTYLIST &&
                CurFunc->Signature.Subtypes.back().Id == TypeId::LIST)
            node.Expr->Ty = CurFunc->Signature.Subtypes.back();
        else if (CurFunc->Signature.Subtypes.back() != node.Expr->Ty) {
            throw Error::Expected("Wrong return type",
                                     CurFunc->Ty.str(),
                                     node.Expr->Ty.str(),
                                     node.Loc);
        }
    }

    void TypeChecker::visit(Or &node) {

        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if (node.Left->Ty.Id == TypeId::BOOL && node.Right->Ty.Id == TypeId::BOOL) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   node);
        }
    }

    void TypeChecker::visit(And &node) {

        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if (node.Left->Ty == TypeId::BOOL && node.Right->Ty == TypeId::BOOL) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   node);
        }
    }

    void TypeChecker::visit(Equal &node) {

        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if (node.Left->Ty == node.Right->Ty) {
            node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   node);
        }
    }

    void TypeChecker::visit(NotEqual &node) {

        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if (node.Left->Ty == node.Right->Ty) {
            node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   node);
        }
    }

    void TypeChecker::visit(Lesser &node) {

        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   node);
        }
    }

    void TypeChecker::visit(Greater &node) {

        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(LesserEq &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(GreaterEq &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Add &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Sub &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Mul &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Div &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Mod &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if ((node.Left->Ty.Id == TypeId::INT && node.Right->Ty.Id == TypeId::INT) ||
            (node.Left->Ty.Id == TypeId::FLOAT && node.Right->Ty.Id == TypeId::FLOAT)) {
            node.Ty = node.Left->Ty;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(ListAdd &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if (node.Right->Ty.Id == TypeId::LIST) {
            if (node.Left->Ty == node.Right->Ty.Subtypes.front()) {
                node.Ty = node.Right->Ty;
            } else {
                throw Error::Binary("Left type must be same type of right List",
                                       node);
            }
        } else if (node.Right->Ty.Id == TypeId::STRING) {
            if (node.Left->Ty.Id == TypeId::CHAR) {
                node.Ty = node.Right->Ty;
            } else {
                throw Error::Binary("Left type must be Char when right is String",
                                       node);
            }
        }  else if (node.Right->Ty.Id == TypeId::EMPTYLIST) {
            node.Ty = Type(TypeId::LIST);
            node.Ty.Subtypes.push_back(node.Left->Ty);
            node.Right->Ty = node.Ty;
        } else {
            throw Error::Binary("Right must be a List",
                                   node);
        }
    }

    void TypeChecker::visit(Par &node) {
        // Visit children
        node.Child->accept(*this);
        // Visit stops here

        // Code starts here
        node.Ty = node.Child->Ty;
        // Code stops here
    }

    void TypeChecker::visit(Not &node) {
        // Visit children
        node.Child->accept(*this);
        // Visit stops here

        if (node.Child->Ty.Id == TypeId::BOOL) {
            node.Ty = node.Child->Ty;
        } else {
            throw Error::Unary("Operator only operates on Bool typed children",
                                  node);
        }
    }

    void TypeChecker::visit(Int &node) {

    }

    void TypeChecker::visit(Float &node) {
    }

    void TypeChecker::visit(Bool &node) {
    }

    void TypeChecker::visit(Char &node) {
    }

    void TypeChecker::visit(String &node) {
    }

    void TypeChecker::visit(ListPattern &node) {
        // Visit children
        for (auto &pattern: node.Patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        if (node.Patterns.size() == 0) {
            node.Ty = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < node.Patterns.size() - 1; ++i) {
                if (node.Patterns[i]->Ty != node.Patterns[i + 1]->Ty) {
                    throw Error::Expected("All items in a List must be of the same type",
                                             node.Patterns[i]->str(),
                                             node.Patterns[i + 1]->str(),
                                             node.Loc);
                }
            }

            node.Ty = Type(TypeId::LIST);
            node.Ty.Subtypes.push_back(node.Patterns[0]->Ty);
        }
    }

    void TypeChecker::visit(TuplePattern &node) {
        // Visit children
        for (auto &pattern: node.Patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        node.Ty = Type(TypeId::TUPLE);

        for (auto &pattern: node.Patterns) {
            node.Ty.Subtypes.push_back(pattern->Ty);
        }
    }

    void TypeChecker::visit(ListSplit &node) {
        // Visit children
        node.Left->accept(*this);
        node.Right->accept(*this);
        // Visit stops here

        if (node.Right->Ty.Id == TypeId::LIST) {
            if (node.Left->Ty == node.Right->Ty.Subtypes.front()) {
                node.Ty = node.Right->Ty;
            } else {
                throw Error::Expected("Left must be the same type as the right Lists children",
                                         node.Right->Ty.Subtypes[0].str(),
                                         node.Left->Ty.str(),
                                         node.Loc);
            }
        } else if (node.Right->Ty.Id == TypeId::STRING) {
            if (node.Left->Ty.Id == TypeId::CHAR) {
                node.Ty = node.Right->Ty;
            } else {
                throw Error::Expected("Left must be type Char, when right is String",
                                         "Char",
                                         node.Left->Ty.str(),
                                         node.Loc);
            }
        } else if (node.Right->Ty.Id == TypeId::EMPTYLIST) {
            node.Ty = Type(TypeId::LIST);
            node.Ty.Subtypes.push_back(node.Left->Ty);
        } else {
            throw Error::Expected("Right must be a List",
                                     "List",
                                     node.Right->Ty.str(),
                                     node.Loc);
        }
    }

    void TypeChecker::visit(List &node) {
        // Visit children
        for (auto &expr: node.Elements) {
            expr->accept(*this);
        }
        // Visit stops here

        if (node.Elements.size() == 0) {
            node.Ty = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < node.Elements.size() - 1; ++i) {
                if (node.Elements[i]->Ty != node.Elements[i + 1]->Ty) {
                    throw Error::Expected("All items in a List must be same type",
                                             node.Elements[i]->str(),
                                             node.Elements[i + 1]->str(),
                                             node.Elements[i + 1]->Loc);
                }
            }
            node.Ty = Type(TypeId::LIST);
            node.Ty.Subtypes.push_back(node.Elements[0]->Ty);
        }
    }

    void TypeChecker::visit(Tuple &node) {
        // Visit children
        for (auto &expr: node.Elements) {
            expr->accept(*this);
        }
        // Visit stops here

        node.Ty = Type(TypeId::TUPLE);

        for (size_t i = 0; i < node.Elements.size(); ++i) {
            node.Ty.Subtypes.push_back(node.Elements[i]->Ty);
        }
    }


    void TypeChecker::visit(Id &node) {
        if (node.Scp->exists(node.Val)) {
            node.Ty = node.Scp->getType(node.Val);
        } else {
            throw Error(node.Val + ": Id does not exist in the current scope",
                           node.Loc);
        }
    }

    void TypeChecker::visit(Call &node) {

        // Visit children
        node.Callee->accept(*this);
        for (auto &expr: node.Args) {
            expr->accept(*this);
        }
        // Visit stops here

        if (node.Callee->Ty.Id != TypeId::SIGNATURE) {
            throw Error::Expected("Can't call a type that is not a Signature",
                         "Signature",
                         node.Callee->Ty.str(),
                         node.Callee->Loc);
        }

        if (node.Args.size() + 1 != node.Callee->Ty.Subtypes.size()) {
            throw Error::Expected("Wrong number of arguments",
                                  to_string(node.Callee->Ty.Subtypes.size() - 1),
                                  to_string(node.Args.size()),
                                  node.Loc);
        }

        for (size_t i = 0; i < node.Args.size(); ++i) {
            if (node.Args[i]->Ty.Id == TypeId::EMPTYLIST &&
                    node.Callee->Ty.Subtypes[i].Id == TypeId::LIST)
                node.Args[i]->Ty = node.Callee->Ty.Subtypes[i];
            else if (node.Args[i]->Ty != node.Callee->Ty.Subtypes[i]) {
                throw Error::Expected("Function was called with an invalid argument",
                                         node.Callee->Ty.Subtypes[i].str(),
                                         node.Args[i]->Ty.str(),
                                         node.Args[i]->Loc);
            }
        }

        node.Ty = node.Callee->Ty.Subtypes.back();
    }
}
