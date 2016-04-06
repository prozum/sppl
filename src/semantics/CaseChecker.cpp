#include <iostream>
#include "CaseChecker.h"
/*
namespace semantics {
    void CaseChecker::visit(common::Program &node) {
        for (auto func : node.funcs) {
            func->accept(*this);
        }

        if(hasError) {
            cerr << "Errors in function cases. Aborting..." << endl;
            exit(1);
        }
    }

    void CaseChecker::visit(common::Function &node) {
        expectedPatternCount = node.types.size() - 1;
        currentFunction = node.id;
        hasDefault = false;

        if (node.cases.size() > 0) {
            for (auto c : node.cases) {
                c->accept(*this);
            }
        } else {
            add_error("No cases found in" + node.id, node.pos);
        }

        if(hasDefault == false) {
            add_error("Error: No default case in" + node.id, node.pos);
        }
    }

    void CaseChecker::visit(common::Case &node) {
        if (node.patterns.size() != expectedPatternCount) {
            hasError = true;
            std::cerr << "Error: Invalid pattern  @ " << currentFunction << " line " << to_string(node.line_no);
            if (node.patterns.size() < expectedPatternCount) {
                std::cerr << " => too few patterns in case, expected " << to_string(expectedPatternCount) << " but got " << to_string(node.patterns.size()) << endl;
            } else {
                std::cerr << " => too many patterns in case, expected " << to_string(expectedPatternCount) << " but got " << to_string(node.patterns.size()) << endl;
            }
            return;
        } else {
            if (hasDefault == true) {
                hasError = true;
                std::cerr << "Error: Unreachable case after default @ " << currentFunction << " line " << to_string(node.line_no) << endl;
            } else {
                bool isDef = true;
                for (auto p : node.patterns) {
                    p->accept(*this);
                    if(!isId) {
                        isDef = false;
                    }
                }
                hasDefault = isDef;
            }
        }
    }

    void CaseChecker::visit(common::Int &node) {
        isId = false;
    }

    void CaseChecker::visit(common::Float &node) {
        isId = false;
    }

    void CaseChecker::visit(common::Char &node) {
        isId = false;
    }

    void CaseChecker::visit(common::String &node) {
        isId = false;
    }

    void CaseChecker::visit(common::Bool &node) {
        isId = false;
    }

    void CaseChecker::visit(common::Id &node) {
        isId = true;
    }
}
*/