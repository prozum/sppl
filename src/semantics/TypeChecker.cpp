//
// Created by hejsil on 3/4/16.
//

#include "TypeChecker.h"
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <cstring>
#include "Scope.h"

using namespace std;

namespace semantics
{
    TypeChecker::TypeChecker() : is_valid(true) { }

    void TypeChecker::visit(Program &node) {
        /* Visit all children */
        for (auto func : node.funcs) {
            func->accept(*this);
        }
        /* Visit stops here */
    }

    void TypeChecker::visit(Function &node) {
        current_func = &node;

        /* Visit children */
        for (auto type : node.types) {
            type->accept(*this);
        }
        for (auto cse : node.cases) {
            try {
                cse->accept(*this);
            } catch (string s) {
                cerr << s << endl;
            }
        }
        /* Visit stops here */
    }

    void TypeChecker::visit(Case &node) {
        /* Visit children */
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        node.expr->accept(*this);
        /* Visit stops here */

        /* code starts here*/
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
                    is_valid = false;
                    stringstream error;
                    Printer printer(error);
                    error << "error - line " << node.patterns[i]->line_no << ": expected ";
                    current_func->types[i]->accept(printer);
                    error << " but was ";
                    node.patterns[i]->node_type->accept(printer);
                    error << "." << endl;
                    node.patterns[i]->accept(printer);
                    throw error.str();
                }
            }
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": expected " << current_func->types.size() - 1;
            error << " patterns, but only " << node.patterns.size() << " was specified." << endl;
            error << "| ";
            for (auto pattern : node.patterns) {
                pattern->accept(printer);
                error << " ";
            }
            error << "=" << endl;

            throw error.str();
        }
    }

    void TypeChecker::visit(Or &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (node.left->node_type->type == Types::BOOL && node.right->node_type->type == Types::BOOL) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"&&\" operator can only operate on Bool typed children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(And &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (node.left->node_type->type == Types::BOOL && node.right->node_type->type == Types::BOOL) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"&&\" operator can only operate on Bool typed children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Equal &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (equal(node.left->node_type, node.right->node_type)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"==\" operator can only operate on children of the same type.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }
        /* Code stops here */
    }

    void TypeChecker::visit(NotEqual &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (equal(node.left->node_type, node.right->node_type)) {
            node.node_type = new Type(Types::BOOL, vector<Type *>(), 0);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"!=\" operator can only operate on children of the same type.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            error << endl;
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Lesser &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"<\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Greater &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \">\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            error << endl;
            throw error.str();
        }
        /* Code stops here */
    }

    void TypeChecker::visit(LesserEq &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"<=\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(GreaterEq &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = new Type(Types::BOOL);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \">=\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Add &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"+\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Sub &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"-\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Mul &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"*\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Div &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"/\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Mod &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if ((node.left->node_type->type == Types::INT && node.right->node_type->type == Types::INT) ||
            (node.left->node_type->type == Types::FLOAT && node.right->node_type->type == Types::FLOAT)) {
            node.node_type = node.left->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"%\" operator can only operate on Int children or Float children.";
            error << " Left type: ";
            node.left->node_type->accept(printer);
            error << ". Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(ListAdd &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (node.right->node_type->type == Types::LIST) {
            if (equal(node.left->node_type, node.right->node_type->types.front())) {
                node.node_type = node.right->node_type;
            } else {
                is_valid = false;
                stringstream error;
                Printer printer(error);
                error << "error - line " << node.line_no << ": Left side of \":\" must be of the same type as the right sides items.";
                error << " Left type: ";
                node.left->node_type->accept(printer);
                error << ". Right type: ";
                node.right->node_type->accept(printer);
                error << endl;
                node.accept(printer);
                throw error.str();
            }
        } else if (node.right->node_type->type == Types::STRING) {
            if (node.left->node_type->type == Types::CHAR) {
                node.node_type = node.right->node_type;
            } else {
                is_valid = false;
                stringstream error;
                Printer printer(error);
                error << "error - line " << node.line_no << ": Left side of \":\" must be type Char, when right side is String.";
                error << " Left type: ";
                node.left->node_type->accept(printer);
                error << ". Right type: ";
                node.right->node_type->accept(printer);
                error << endl;
                node.accept(printer);
                throw error.str();
            }
        }  else if (node.right->node_type->type == Types::EMPTYLIST) {
            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.left->node_type);
            node.right->node_type = node.node_type;
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \":\" operators right side must be a list.";
            error << " Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Par &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.child->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        node.node_type = node.child->node_type;
        /* Code stops here */
    }

    void TypeChecker::visit(Not &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.child->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (node.child->node_type->type == Types::BOOL) {
            node.node_type = node.child->node_type;
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \"!\" operator can only operate on a Bool typed child.";
            error << " Child type: ";
            node.child->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }
        /* Code stops here */
    }

    void TypeChecker::visit(Int &node) {
        /* Code goes here */
        node.node_type = new Type(Types::INT);
        garbage.push_back(node.node_type);

        /* Code stops here */
    }

    void TypeChecker::visit(Float &node) {
        /* Code goes here */
        node.node_type = new Type(Types::FLOAT);
        garbage.push_back(node.node_type);

        /* Code stops here */
    }

    void TypeChecker::visit(Bool &node) {
        /* Code goes here */
        node.node_type = new Type(Types::BOOL);
        garbage.push_back(node.node_type);

        /* Code stops here */
    }

    void TypeChecker::visit(Char &node) {
        /* Code goes here */
        node.node_type = new Type(Types::CHAR);
        garbage.push_back(node.node_type);

        /* Code stops here */
    }

    void TypeChecker::visit(String &node) {
        /* Code goes here */

        node.node_type = new Type(Types::STRING);
        garbage.push_back(node.node_type);

        /* Code stops here */
    }

    void TypeChecker::visit(ListPattern &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        /* Visit stops here */

        /* Code goes here */
        if (node.patterns.size() == 0) {
            node.node_type = new Type(Types::EMPTYLIST);
            garbage.push_back(node.node_type);
        } else {
            for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
                if (!equal(node.patterns[i]->node_type, node.patterns[i + 1]->node_type)) {
                    is_valid = false;
                    stringstream error;
                    Printer printer(error);
                    error << "error - line " << node.line_no << ": All items in a list must be of the same type.";
                    error << " List type: ";
                    node.patterns[i]->node_type->accept(printer);
                    error << ". Child type: ";
                    node.patterns[i + 1]->node_type->accept(printer);
                    error << endl;
                    node.accept(printer);
                    throw error.str();
                }
            }

            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.patterns[0]->node_type);
            garbage.push_back(node.node_type);
        }
        /* Code stops here */
    }

    void TypeChecker::visit(TuplePattern &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        /* Visit stops here */

        /* Code goes here */
        node.node_type = new Type(Types::TUPLE);
        garbage.push_back(node.node_type);

        for (auto pattern : node.patterns) {
            node.node_type->types.push_back(pattern->node_type);
        }
        /* Code stops here */
    }

    void TypeChecker::visit(ListSplit &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.left->accept(*this);
        node.right->accept(*this);
        /* Visit stops here */

        /* Code goes here */
        if (node.right->node_type->type == Types::LIST) {
            if (equal(node.left->node_type, node.right->node_type->types.front())) {
                node.node_type = node.right->node_type;
            } else {
                is_valid = false;
                stringstream error;
                Printer printer(error);
                error << "error - line " << node.line_no << ": Left side of \":\" must be of the same type as the right sides items.";
                error << " Left type: ";
                node.left->node_type->accept(printer);
                error << ". Right type: ";
                node.right->node_type->accept(printer);
                error << endl;
                node.accept(printer);
                throw error.str();
            }
        } else if (node.right->node_type->type == Types::STRING) {
            if (node.left->node_type->type == Types::CHAR) {
                node.node_type = node.right->node_type;
            } else {
                is_valid = false;
                stringstream error;
                Printer printer(error);
                error << "error - line " << node.line_no << ": Left side of \":\" must be type Char, when right side is String.";
                error << " Left type: ";
                node.left->node_type->accept(printer);
                error << ". Right type: ";
                node.right->node_type->accept(printer);
                error << endl;
                node.accept(printer);
                throw error.str();
            }
        } else if (node.right->node_type->type == Types::EMPTYLIST) {
            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.left->node_type);
            garbage.push_back(node.node_type);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": \":\" operators right side must be a list.";
            error << " Right type: ";
            node.right->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }
        /* Code stops here */
    }

    void TypeChecker::visit(List &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        /* Visit stops here */

        /* Code goes here */
        if (node.exprs.size() == 0) {
            node.node_type = new Type(Types::EMPTYLIST);
            garbage.push_back(node.node_type);
        } else {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                if (!equal(node.exprs[i]->node_type, node.exprs[i + 1]->node_type)) {
                    is_valid = false;
                    stringstream error;
                    Printer printer(error);
                    error << "error - line " << node.line_no << ": All items in a list must be of the same type.";
                    error << " List type: ";
                    node.exprs[i]->node_type->accept(printer);
                    error << ". Child type: ";
                    node.exprs[i + 1]->node_type->accept(printer);
                    error << endl;
                    node.accept(printer);
                    throw error.str();
                }
            }
            node.node_type = new Type(Types::LIST);
            node.node_type->types.push_back(node.exprs[0]->node_type);
            garbage.push_back(node.node_type);
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Tuple &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        /* Visit stops here */

        /* Code goes here */
        node.node_type = new Type(Types::TUPLE);
        garbage.push_back(node.node_type);

        for (size_t i = 0; i < node.exprs.size(); ++i) {
            node.node_type->types.push_back(node.exprs[i]->node_type);
        }
        /* Code stops here */
    }


    void TypeChecker::visit(Id &node) {
        /* Code goes here */
        if (node.scope->exists(node.id)) {
            node.node_type = node.scope->get_type(node.id);
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": Id does not exist in the current scope.";
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Call &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        node.callee->accept(*this);
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
        /* Visit stops here */

        /* Code goes here */
        if (node.callee->node_type->type == Types::SIGNATURE) {
            if (node.exprs.size() + 1 == node.callee->node_type->types.size()) {
                for (size_t i = 0; i < node.exprs.size(); ++i) {
                    if (!equal(node.exprs[i]->node_type, node.callee->node_type->types[i])) {
                        is_valid = false;
                        stringstream error;
                        Printer printer(error);
                        error << "error - line " << node.line_no << ": Function was called with an invalid argument.";
                        error << " Expected type: ";
                        node.callee->node_type->types[i]->accept(printer);
                        error << ". Argument type: ";
                        node.exprs[i]->node_type->accept(printer);
                        error << endl;
                        node.accept(printer);
                        throw error.str();
                    }
                }
            } else {
                is_valid = false;
                stringstream error;
                Printer printer(error);
                error << "error - line " << node.line_no << ": Wrong number of arguments. Expected ";
                error << node.callee->node_type->types.size() - 1 << " arguments but was called with";
                error << node.exprs.size() << "." << endl;
                node.accept(printer);
                throw error.str();
            }
            node.node_type = node.callee->node_type->types.back();
        } else {
            is_valid = false;
            stringstream error;
            Printer printer(error);
            error << "error - line " << node.line_no << ": Can't call a type that is not a Signature.";
            error << " Callee type: ";
            node.callee->node_type->accept(printer);
            error << endl;
            node.accept(printer);
            throw error.str();
        }

        /* Code stops here */
    }

    void TypeChecker::visit(Type &node) {
        /* Code goes here */


        /* Code stops here */

        /* Visit children */
        for (auto type : node.types) {
            type->accept(*this);
        }
        /* Visit stops here */

        /* Code goes here */
        node.node_type = &node;

        /* Code stops here */
    }

    bool TypeChecker::equal(Type *type1, Type *type2) {
        return *type1 == *type2;
    }
}
