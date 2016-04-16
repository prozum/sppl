#include "PatternChecker.h"
namespace semantics {
    PatternChecker::PatternChecker() { }

    void PatternChecker::visit(Program &node) {
        // Visit children
        for (auto &func: node.Funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void PatternChecker::visit(Function &node) {
        for (auto &cse: node.Cases) {
            cse->accept(*this);
        }
    }

    void PatternChecker::visit(Case &node) {
        if (node.Patterns.size() != cur_func->Signature.Subtypes.size() - 1) {
            throw Error::Expected("Case didn't have the correct number of patterns",
                                  to_string(cur_func->Signature.Subtypes.size() - 1),
                                  to_string(node.Patterns.size()),
                                  node.Loc);
        }
    }
}
