#include "PatternChecker.h"
#include <iostream>
/*
            throw add_error_expected("Case didn't have the correct number of patterns",
                  current_func->types.size() - 1,
                  node.patterns.size(),
                  node.pos);
*/

/*
namespace semantics {
    PatternChecker::PatternChecker() { }

    void PatternChecker::visit(Program &node) {
        // Visit children
        for (auto func : node.funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void PatternChecker::visit(Function &node) {
        // Visit children
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
        // Visit stops here
    }

    void PatternChecker::visit(Case &node) {
        if (node.patterns.size() == cur_func->types.size() - 1) {

            // Visit children
            for (auto &pattern : node.patterns) {
                pattern->accept(*this);
            }
            // Visit stops here

        } else {
            AddError(Error::Expected("Case didn't have the correct number of patterns",
                                     to_string(cur_func->types.size() - 1),
                                     to_string(node.patterns.size()),
                                     node.loc);
            return;
        }
    }

    void PatternChecker::visit(ListPattern &node) {
        // Visit children
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
        // Visit stops here
    }

    void PatternChecker::visit(TuplePattern &node) {
        // Visit children
        for (size_t i = 0; i < node.patterns.size(); i++) {
            node.patterns[i]->accept(*this);
        }
        // Visit stops here
    }

    void PatternChecker::visit(ListSplit &node) {
        // Visit children
        node.left->accept(*this);
        node.right->accept(*this);
        // Visit stops here
    }
}
*/