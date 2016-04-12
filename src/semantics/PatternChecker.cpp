#include "PatternChecker.h"
namespace semantics {
    PatternChecker::PatternChecker() { }

    void PatternChecker::visit(Program &node) {
        // Visit children
        for (auto &func: node.funcs) {
            func->accept(*this);
        }
        // Visit stops here
    }

    void PatternChecker::visit(Function &node) {
        for (auto &cse: node.cases) {
            cse->accept(*this);
        }
    }

    void PatternChecker::visit(Case &node) {
        if (node.patterns.size() != cur_func->signature.subtypes.size() - 1) {
            AddError(Error::Expected("Case didn't have the correct number of patterns",
                                     to_string(cur_func->signature.subtypes.size() - 1),
                                     to_string(node.patterns.size()),
                                     node.loc));
            return;
        }
    }
}
