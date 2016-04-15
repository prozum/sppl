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
        for (auto &func: node.funcs) {
            func->accept(*this);

            if (func->id == "main") {
                if (func->signature.subtypes.front() != strlist) {
                    addError(Error::Expected("Declaration of \"main\" had wrong input type",
                                             strlist.str(),
                                             func->signature.subtypes.front().str(),
                                             func->loc));
                } else if (func->signature.subtypes.size() != 2) {
                    addError(Error::Expected("Function \"main\" had wrong number of input",
                                             "2",
                                             to_string(func->signature.subtypes.size()),
                                             func->loc));
                }
            }

            Safe;
        }
        // Visit stops here
    }

    void TypeChecker::visit(Function &node) {
        current_func = &node;

        node.type = node.signature.subtypes.back();

        // Visit children
        for (auto &cse: node.cases) {
            cse->accept(*this);
            Safe;
        }
        // Visit stops here
    }

    void TypeChecker::visit(Case &node) {
        // Visit children
        for (auto &pattern: node.patterns) {
            pattern->accept(*this);
        }
        node.expr->accept(*this);
        CheckPanic;

        if (current_func->is_anon) {
            current_func->type = node.expr->type;
            current_func->signature = Type(TypeId::SIGNATURE, vector<Type>());
            current_func->signature.subtypes.push_back(node.expr->type);
        }

        if (node.patterns.size() == current_func->signature.subtypes.size() - 1) {
            for (size_t i = 0; i < node.patterns.size(); ++i) {
                if (node.patterns[i]->type.id == TypeId::EMPTYLIST &&
                        current_func->signature.subtypes[i].id == TypeId::LIST) {
                    node.patterns[i]->type = current_func->signature.subtypes[i];
                }

                if (node.patterns[i]->type != current_func->signature.subtypes[i]) {
                    addError(Error::Expected("Wrong pattern type",
                                             current_func->signature.subtypes[i].str(),
                                             node.patterns[i]->type.str(),
                                             node.patterns[i]->loc));
                    return;
                }
            }
        } else {
            addError(Error::Expected("Wrong pattern count",
                                     to_string(current_func->signature.subtypes.size() - 1),
                                     to_string(node.patterns.size()),
                                     node.loc));
            return;
        }

        if (node.expr->type.id == TypeId::EMPTYLIST &&
                current_func->signature.subtypes.back().id == TypeId::LIST)
            node.expr->type = current_func->signature.subtypes.back();
        else if (current_func->type != node.expr->type) {
            addError(Error::Expected("Wrong return type",
                                     current_func->type.str(),
                                     node.expr->type.str(),
                                     node.loc));
            return;
        }
    }

    void TypeChecker::visit(Or &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.left->type.id == TypeId::BOOL && node.right->type.id == TypeId::BOOL) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Bool typed children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(And &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.left->type == TypeId::BOOL && node.right->type == TypeId::BOOL) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Bool typed children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Equal &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.left->type == node.right->type) {
            node.type = Type(TypeId::BOOL);
        } else {
            addError(Error::Binary("Operator only operates on children of the same type",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(NotEqual &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.left->type == node.right->type) {
            node.type = Type(TypeId::BOOL);
        } else {
            addError(Error::Binary("Operator only operates on children of the same type",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Lesser &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            addError(Error::Binary("Operator only operates on children of the same type",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Greater &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(LesserEq &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(GreaterEq &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Add &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Sub &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Mul &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Div &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Mod &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            addError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(ListAdd &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.right->type.id == TypeId::LIST) {
            if (node.left->type == node.right->type.subtypes.front()) {
                node.type = node.right->type;
            } else {
                addError(Error::Binary("Left type must be same type of right List",
                                       node));
                return;
            }
        } else if (node.right->type.id == TypeId::STRING) {
            if (node.left->type.id == TypeId::CHAR) {
                node.type = node.right->type;
            } else {
                addError(Error::Binary("Left type must be Char when right is String",
                                       node));
                return;
            }
        }  else if (node.right->type.id == TypeId::EMPTYLIST) {
            node.type = Type(TypeId::LIST);
            node.type.subtypes.push_back(node.left->type);
            node.right->type = node.type;
        } else {
            addError(Error::Binary("Right must be a List",
                                   node));
            return;
        }
    }

    void TypeChecker::visit(Par &node) {
        // Visit children
        node.child->accept(*this);
        CheckPanic;
        // Visit stops here

        // Code starts here
        node.type = node.child->type;
        // Code stops here
    }

    void TypeChecker::visit(Not &node) {
        // Visit children
        node.child->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.child->type.id == TypeId::BOOL) {
            node.type = node.child->type;
        } else {
            addError(Error::Unary("Operator only operates on Bool typed children",
                                  node));
            return;
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
        for (auto &pattern: node.patterns) {
            pattern->accept(*this);
        }
        CheckPanic;
        // Visit stops here

        if (node.patterns.size() == 0) {
            node.type = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
                if (node.patterns[i]->type != node.patterns[i + 1]->type) {
                    addError(Error::Expected("All items in a List must be of the same type",
                                             node.patterns[i]->str(),
                                             node.patterns[i + 1]->str(),
                                             node.loc));
                    return;
                }
            }

            node.type = Type(TypeId::LIST);
            node.type.subtypes.push_back(node.patterns[0]->type);
        }
    }

    void TypeChecker::visit(TuplePattern &node) {
        // Visit children
        for (auto &pattern: node.patterns) {
            pattern->accept(*this);
        }
        CheckPanic;
        // Visit stops here

        node.type = Type(TypeId::TUPLE);

        for (auto &pattern: node.patterns) {
            node.type.subtypes.push_back(pattern->type);
        }
    }

    void TypeChecker::visit(ListSplit &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        CheckPanic;
        // Visit stops here

        if (node.right->type.id == TypeId::LIST) {
            if (node.left->type == node.right->type.subtypes.front()) {
                node.type = node.right->type;
            } else {
                addError(Error::Expected("Left must be the same type as the right Lists children",
                                         node.right->type.subtypes[0].str(),
                                         node.left->type.str(),
                                         node.loc));
                return;
            }
        } else if (node.right->type.id == TypeId::STRING) {
            if (node.left->type.id == TypeId::CHAR) {
                node.type = node.right->type;
            } else {
                addError(Error::Expected("Left must be type Char, when right is String",
                                         "Char",
                                         node.left->type.str(),
                                         node.loc));
                return;
            }
        } else if (node.right->type.id == TypeId::EMPTYLIST) {
            node.type = Type(TypeId::LIST);
            node.type.subtypes.push_back(node.left->type);
        } else {
            addError(Error::Expected("Right must be a List",
                                     "List",
                                     node.right->type.str(),
                                     node.loc));
            return;
        }
    }

    void TypeChecker::visit(List &node) {
        // Visit children
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        CheckPanic;
        // Visit stops here

        if (node.exprs.size() == 0) {
            node.type = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                if (node.exprs[i]->type != node.exprs[i + 1]->type) {
                    addError(Error::Expected("All items in a List must be same type",
                                             node.exprs[i]->str(),
                                             node.exprs[i + 1]->str(),
                                             node.exprs[i + 1]->loc));
                    return;
                }
            }
            node.type = Type(TypeId::LIST);
            node.type.subtypes.push_back(node.exprs[0]->type);
        }
    }

    void TypeChecker::visit(Tuple &node) {
          // Visit children
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        CheckPanic;
        // Visit stops here

        node.type = Type(TypeId::TUPLE);

        for (size_t i = 0; i < node.exprs.size(); ++i) {
            node.type.subtypes.push_back(node.exprs[i]->type);
        }
    }


    void TypeChecker::visit(Id &node) {
        if (node.scope->exists(node.id)) {
            node.type = node.scope->get_type(node.id);
        } else {
            addError(Error(node.id + ": Id does not exist in the current scope",
                           node.loc));
            return;
        }
    }

    void TypeChecker::visit(Call &node) {

        // Visit children
        node.callee->accept(*this);
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        CheckPanic;
        // Visit stops here

        if (node.callee->type.id == TypeId::SIGNATURE) {
            if (node.exprs.size() + 1 == node.callee->type.subtypes.size()) {
                for (size_t i = 0; i < node.exprs.size(); ++i) {
                    if (node.exprs[i]->type.id == TypeId::EMPTYLIST &&
                            node.callee->type.subtypes[i].id == TypeId::LIST)
                        node.exprs[i]->type = node.callee->type.subtypes[i];
                    else if (node.exprs[i]->type != node.callee->type.subtypes[i]) {
                        addError(Error::Expected("Function was called with an invalid argument",
                                                 node.callee->type.subtypes[i].str(),
                                                 node.exprs[i]->type.str(),
                                                 node.exprs[i]->loc));
                        return;
                    }
                }
            } else {
                addError(Error::Expected("Wrong number of arguments",
                                         to_string(node.callee->type.subtypes.size() - 1),
                                         to_string(node.exprs.size()),
                                         node.loc));
                return;
            }
            node.type = node.callee->type.subtypes.back();
        } else {
            addError(Error::Expected("Can't call a type that is not a Signature",
                                     "Signature",
                                     node.callee->type.str(),
                                     node.callee->loc));
            return;
        }
    }
}
