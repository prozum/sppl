#include "TypeChecker.h"
#include "Error.h"
#include "Scope.h"

#include <iostream>

using namespace std;
namespace semantics
{
    TypeChecker::TypeChecker() { }

    void TypeChecker::visit(Program &node) {
        // Visit children
        for (auto func : node.funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void TypeChecker::visit(Function &node) {
        current_func = &node;

        // Visit children
        for (auto type : node.types) {
            type->accept(*this);
        }

        if (node.types.size() > 0)
            node.node_type = node.types[0];

        for (auto cse : node.cases) {
            cse->accept(*this);
        }
        // Visit stops here
    }

    void TypeChecker::visit(Case &node) {
        // Visit children
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        node.expr->accept(*this);
        // Visit stops here

        // Code starts here
        if (current_func->id.compare(ANON_FUNC_NAME) == 0) {
            current_func->node_type = node.expr->node_type;
            current_func->types.push_back(node.expr->node_type);
        }
        if (node.patterns.size() + 1 == current_func->types.size()) {
            for (size_t i = 0; i < node.patterns.size(); ++i) {
                if (node.patterns[i]->node_type->type == Types::EMPTYLIST) {
                    node.patterns[i]->node_type = current_func->types[i];
                }

                if (!equal(node.patterns[i]->node_type, current_func->types[i])) {
                    AddError(Error::Expected("Wrong pattern type",
                                current_func->types[i]->str(),
                                node.patterns[i]->node_type->str(),
                                node.patterns[i]->node_type->loc));
                    return;
                }
            }
        } else {
            AddError(Error::Expected("Wrong pattern count",
                        to_string(current_func->types.size() - 1),
                        to_string(node.patterns.size()),
                        node.loc));
            return;
        }
    }

    void TypeChecker::visit(Or &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if (node.left->node_type->type == Types::BOOL && node.right->node_type->type == Types::BOOL) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Bool typed children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(And &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if (node.left->node_type->type == Types::BOOL && node.right->node_type->type == Types::BOOL) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Bool typed children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Equal &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if (equal(node.left->node_type, node.right->node_type)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Operator only operates on children of the same type",
                                   node));
            return;
        }
        // Code stops here
    }

    void TypeChecker::visit(NotEqual &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if (equal(node.left->node_type, node.right->node_type)) {
            node.node_type = new Type(Types::BOOL, vector<Type *>());
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Operator only operates on children of the same type",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Lesser &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Operator only operates on children of the same type",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Greater &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
        // Code stops here
    }

    void TypeChecker::visit(LesserEq &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }
        // Code stops here
    }

    void TypeChecker::visit(GreaterEq &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Add &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Sub &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Mul &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Div &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Mod &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            AddError(Error::Binary("Operator only operates on Int or Float children",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(ListAdd &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if (node.right->node_type->type == Types::LIST) {
            if (equal(node.left->node_type, node.right->node_type->types.front())) {
                node.node_type = node.right->node_type;
            } else {
                AddError(Error::Binary("Left type must be same type of right List",
                                       node));
                return;
            }
        } else if (node.right->node_type->type == Types::STRING) {
            if (node.left->node_type->type == Types::CHAR) {
                node.node_type = node.right->node_type;
            } else {
                AddError(Error::Binary("Left type must be Char when right is String",
                                       node));
                return;
            }
        }  else if (node.right->node_type->type == Types::EMPTYLIST) {
            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.left->node_type);
            node.right->node_type = node.node_type;
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Binary("Right must be a List",
                                   node));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Par &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.child->accept(*this);
        // Visit stops here

        // Code starts here
        node.node_type = node.child->node_type;
        // Code stops here
    }

    void TypeChecker::visit(Not &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.child->accept(*this);
        // Visit stops here

        // Code starts here
        if (node.child->node_type->type == Types::BOOL) {
            node.node_type = node.child->node_type;
        } else {
            AddError(Error::Unary("Operator only operates on Bool typed children",
                                   node));
            return;
        }
        // Code stops here
    }

    void TypeChecker::visit(Int &node) {
        // Code starts here
        node.node_type = new Type(Types::INT);
        garbage.push_back(node.node_type);

        // Code stops here
    }

    void TypeChecker::visit(Float &node) {
        // Code starts here
        node.node_type = new Type(Types::FLOAT);
        garbage.push_back(node.node_type);

        // Code stops here
    }

    void TypeChecker::visit(Bool &node) {
        // Code starts here
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);

        // Code stops here
    }

    void TypeChecker::visit(Char &node) {
        // Code starts here
        node.node_type = new Type(Types::CHAR);
        garbage.push_back(node.node_type);

        // Code stops here
    }

    void TypeChecker::visit(String &node) {
        // Code starts here

        node.node_type = new Type(Types::STRING);
        garbage.push_back(node.node_type);

        // Code stops here
    }

    void TypeChecker::visit(ListPattern &node) {
        // Code starts here


        // Code stops here

        // Visit children
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        // Code starts here
        if (node.patterns.size() == 0) {
            node.node_type = new Type(Types::EMPTYLIST);
            garbage.push_back(node.node_type);
        } else {
            for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
                if (!equal(node.patterns[i]->node_type, node.patterns[i + 1]->node_type)) {
                    AddError(Error::Expected("All items in a List must be of the same type",
                                             node.patterns[i]->str(),
                                             node.patterns[i + 1]->str(),
                                             node.loc));
                    return;
                }
            }

            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.patterns[0]->node_type);
            garbage.push_back(node.node_type);
        }
        // Code stops here
    }

    void TypeChecker::visit(TuplePattern &node) {
        // Code starts here


        // Code stops here

        // Visit children
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        // Visit stops here

        // Code starts here
        node.node_type = new Type(Types::TUPLE);
        garbage.push_back(node.node_type);

        for (auto pattern : node.patterns) {
            node.node_type->types.push_back(pattern->node_type);
        }
        // Code stops here
    }

    void TypeChecker::visit(ListSplit &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here

        // Code starts here
        if (node.right->node_type->type == Types::LIST) {
            if (equal(node.left->node_type, node.right->node_type->types.front())) {
                node.node_type = node.right->node_type;
            } else {
                AddError(Error::Expected("Left must be the same type as the right List",
                                         node.right->node_type->types[0]->str(),
                                         node.right->str(),
                                         node.loc));
                return;
            }
        } else if (node.right->node_type->type == Types::STRING) {
            if (node.left->node_type->type == Types::CHAR) {
                node.node_type = node.right->node_type;
            } else {
                AddError(Error::Expected("Left must be type Char, when right is String",
                                         "Char",
                                         node.left->node_type->str(),
                                         node.loc));
                return;
            }
        } else if (node.right->node_type->type == Types::EMPTYLIST) {
            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.left->node_type);
            garbage.push_back(node.node_type);
        } else {
            AddError(Error::Expected("Right must be a List",
                                     "List",
                                     node.right->node_type->str(),
                                     node.loc));
            return;
        }
        // Code stops here
    }

    void TypeChecker::visit(List &node) {
        // Code starts here


        // Code stops here

        // Visit children
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here

        // Code starts here
        if (node.exprs.size() == 0) {
            node.node_type = new Type(Types::EMPTYLIST);
            garbage.push_back(node.node_type);
        } else {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                if (!equal(node.exprs[i]->node_type, node.exprs[i + 1]->node_type)) {
                    AddError(Error::Expected("All items in a List must be same type",
                                             node.exprs[i]->str(),
                                             node.exprs[i + 1]->str(),
                                             node.exprs[i + 1]->loc));
                    return;
                }
            }
            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.exprs[0]->node_type);
            garbage.push_back(node.node_type);
        }

        // Code stops here
    }

    void TypeChecker::visit(Tuple &node) {
        // Code starts here


        // Code stops here

        // Visit children
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here

        // Code starts here
        node.node_type = new Type(Types::TUPLE);
        garbage.push_back(node.node_type);

        for (size_t i = 0; i < node.exprs.size(); ++i) {
            node.node_type->types.push_back(node.exprs[i]->node_type);
        }
        // Code stops here
    }


    void TypeChecker::visit(Id &node) {
        // Code starts here
        if (node.scope->exists(node.id)) {
            node.node_type = node.scope->get_type(node.id);
        } else {
            AddError(Error(node.id + ": Id does not exist in the current scope",
                           node.loc));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Call &node) {
        // Code starts here


        // Code stops here

        // Visit children
        node.callee->accept(*this);
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        // Visit stops here

        // Code starts here
        if (node.callee->node_type->type == Types::SIGNATURE) {
            if (node.exprs.size() + 1 == node.callee->node_type->types.size()) {
                for (size_t i = 0; i < node.exprs.size(); ++i) {
                    if (!equal(node.exprs[i]->node_type, node.callee->node_type->types[i])) {
                        AddError(Error::Expected("Function was called with an invalid argument",
                                                 node.callee->node_type->types[i]->str(),
                                                 node.exprs[i]->node_type->str(),
                                                 node.exprs[i]->loc));
                        return;
                    }
                }
            } else {
                AddError(Error::Expected("Wrong number of arguments",
                                         to_string(node.callee->node_type->types.size() - 1),
                                         to_string(node.exprs.size()),
                                         node.loc));
                return;
            }
            node.node_type = node.callee->node_type->types.back();
        } else {
            AddError(Error::Expected("Can't call a type that is not a Signature",
                                     "Signature",
                                     node.callee->node_type->str(),
                                     node.callee->loc));
            return;
        }

        // Code stops here
    }

    void TypeChecker::visit(Type &node) {
        // Code starts here


        // Code stops here

        // Visit children
        for (auto type : node.types) {
            type->accept(*this);
        }
        // Visit stops here

        // Code starts here
        node.node_type = &node;

        // Code stops here
    }

    bool TypeChecker::equal(Type *type1, Type *type2) {
        return *type1 == *type2;
    }
}
