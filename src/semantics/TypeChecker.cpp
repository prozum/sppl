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

            try {
                if (func->id == "main") {
                    if (func->signature.subtypes.front() != strlist) {
                        throw Error::Expected("Declaration of \"main\" had wrong input type",
                                              strlist.str(),
                                              func->signature.subtypes.front().str(),
                                              func->loc);
                    } else if (func->signature.subtypes.size() != 2) {
                        throw Error::Expected("Function \"main\" had wrong number of input",
                                              "2",
                                              to_string(func->signature.subtypes.size()),
                                              func->loc);
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
        current_func = &node;

        // Visit children
        for (auto &cse: node.cases) {
            try {
                cse->accept(*this);
            }
            catch (Error err) {
                Errors.push_back(err);
            }
        }
        // Visit stops here

        //auto t = !hasError();
        // Set return type
        if (!hasError())
            node.type = node.signature.subtypes.back();
    }

    void TypeChecker::visit(Case &node) {
        // Visit children
        for (auto &pattern: node.patterns) {
            pattern->accept(*this);
        }
        node.expr->accept(*this);

        // Set signature for anonymous function
        if (current_func->is_anon) {
            current_func->signature = Type(TypeId::SIGNATURE, vector<Type>({node.expr->type}));
        }

        if (node.patterns.size() == current_func->signature.subtypes.size() - 1) {
            for (size_t i = 0; i < node.patterns.size(); ++i) {
                if (node.patterns[i]->type.id == TypeId::EMPTYLIST &&
                        current_func->signature.subtypes[i].id == TypeId::LIST) {
                    node.patterns[i]->type = current_func->signature.subtypes[i];
                }

                if (node.patterns[i]->type != current_func->signature.subtypes[i]) {
                    throw Error::Expected("Wrong pattern type",
                                             current_func->signature.subtypes[i].str(),
                                             node.patterns[i]->type.str(),
                                             node.patterns[i]->loc);
                }
            }
        } else {
            throw Error::Expected("Wrong pattern count",
                                     to_string(current_func->signature.subtypes.size() - 1),
                                     to_string(node.patterns.size()),
                                     node.loc);
        }

        if (node.expr->type.id == TypeId::EMPTYLIST &&
                current_func->signature.subtypes.back().id == TypeId::LIST)
            node.expr->type = current_func->signature.subtypes.back();
        else if (current_func->signature.subtypes.back() != node.expr->type) {
            throw Error::Expected("Wrong return type",
                                     current_func->type.str(),
                                     node.expr->type.str(),
                                     node.loc);
        }
    }

    void TypeChecker::visit(Or &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if (node.left->type.id == TypeId::BOOL && node.right->type.id == TypeId::BOOL) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   node);
        }
    }

    void TypeChecker::visit(And &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if (node.left->type == TypeId::BOOL && node.right->type == TypeId::BOOL) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Bool typed children",
                                   node);
        }
    }

    void TypeChecker::visit(Equal &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if (node.left->type == node.right->type) {
            node.type = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   node);
        }
    }

    void TypeChecker::visit(NotEqual &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if (node.left->type == node.right->type) {
            node.type = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   node);
        }
    }

    void TypeChecker::visit(Lesser &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on children of the same type",
                                   node);
        }
    }

    void TypeChecker::visit(Greater &node) {

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(LesserEq &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(GreaterEq &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = Type(TypeId::BOOL);
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Add &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Sub &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Mul &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Div &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(Mod &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if ((node.left->type.id == TypeId::INT && node.right->type.id == TypeId::INT) ||
            (node.left->type.id == TypeId::FLOAT && node.right->type.id == TypeId::FLOAT)) {
            node.type = node.left->type;
        } else {
            throw Error::Binary("Operator only operates on Int or Float children",
                                   node);
        }
    }

    void TypeChecker::visit(ListAdd &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        if (node.right->type.id == TypeId::LIST) {
            if (node.left->type == node.right->type.subtypes.front()) {
                node.type = node.right->type;
            } else {
                throw Error::Binary("Left type must be same type of right List",
                                       node);
            }
        } else if (node.right->type.id == TypeId::STRING) {
            if (node.left->type.id == TypeId::CHAR) {
                node.type = node.right->type;
            } else {
                throw Error::Binary("Left type must be Char when right is String",
                                       node);
            }
        }  else if (node.right->type.id == TypeId::EMPTYLIST) {
            node.type = Type(TypeId::LIST);
            node.type.subtypes.push_back(node.left->type);
            node.right->type = node.type;
        } else {
            throw Error::Binary("Right must be a List",
                                   node);
        }
    }

    void TypeChecker::visit(Par &node) {
        // Visit children
        node.child->accept(*this);
        // Visit stops here

        // Code starts here
        node.type = node.child->type;
        // Code stops here
    }

    void TypeChecker::visit(Not &node) {
        // Visit children
        node.child->accept(*this);
        // Visit stops here

        if (node.child->type.id == TypeId::BOOL) {
            node.type = node.child->type;
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
        for (auto &pattern: node.patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        if (node.patterns.size() == 0) {
            node.type = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
                if (node.patterns[i]->type != node.patterns[i + 1]->type) {
                    throw Error::Expected("All items in a List must be of the same type",
                                             node.patterns[i]->str(),
                                             node.patterns[i + 1]->str(),
                                             node.loc);
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
        // Visit stops here

        if (node.right->type.id == TypeId::LIST) {
            if (node.left->type == node.right->type.subtypes.front()) {
                node.type = node.right->type;
            } else {
                throw Error::Expected("Left must be the same type as the right Lists children",
                                         node.right->type.subtypes[0].str(),
                                         node.left->type.str(),
                                         node.loc);
            }
        } else if (node.right->type.id == TypeId::STRING) {
            if (node.left->type.id == TypeId::CHAR) {
                node.type = node.right->type;
            } else {
                throw Error::Expected("Left must be type Char, when right is String",
                                         "Char",
                                         node.left->type.str(),
                                         node.loc);
            }
        } else if (node.right->type.id == TypeId::EMPTYLIST) {
            node.type = Type(TypeId::LIST);
            node.type.subtypes.push_back(node.left->type);
        } else {
            throw Error::Expected("Right must be a List",
                                     "List",
                                     node.right->type.str(),
                                     node.loc);
        }
    }

    void TypeChecker::visit(List &node) {
        // Visit children
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here

        if (node.exprs.size() == 0) {
            node.type = Type(TypeId::EMPTYLIST);
        } else {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                if (node.exprs[i]->type != node.exprs[i + 1]->type) {
                    throw Error::Expected("All items in a List must be same type",
                                             node.exprs[i]->str(),
                                             node.exprs[i + 1]->str(),
                                             node.exprs[i + 1]->loc);
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
            throw Error(node.id + ": Id does not exist in the current scope",
                           node.loc);
        }
    }

    void TypeChecker::visit(Call &node) {

        // Visit children
        node.callee->accept(*this);
        for (auto &expr: node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here

        if (node.callee->type.id == TypeId::SIGNATURE) {
            if (node.exprs.size() + 1 == node.callee->type.subtypes.size()) {
                for (size_t i = 0; i < node.exprs.size(); ++i) {
                    if (node.exprs[i]->type.id == TypeId::EMPTYLIST &&
                            node.callee->type.subtypes[i].id == TypeId::LIST)
                        node.exprs[i]->type = node.callee->type.subtypes[i];
                    else if (node.exprs[i]->type != node.callee->type.subtypes[i]) {
                        throw Error::Expected("Function was called with an invalid argument",
                                                 node.callee->type.subtypes[i].str(),
                                                 node.exprs[i]->type.str(),
                                                 node.exprs[i]->loc);
                    }
                }
            } else {
                throw Error::Expected("Wrong number of arguments",
                                         to_string(node.callee->type.subtypes.size() - 1),
                                         to_string(node.exprs.size()),
                                         node.loc);
            }
            node.type = node.callee->type.subtypes.back();
        } else {
            throw Error::Expected("Can't call a type that is not a Signature",
                                     "Signature",
                                     node.callee->type.str(),
                                     node.callee->loc);
        }
    }
}
