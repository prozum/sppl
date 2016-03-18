#include <iostream>
#include "CaseChecker.h"

namespace semantics {
    void CaseChecker::visit(common::Program &node) {
        for (auto func : node.funcs) {
            func->accept(*this);
        }
    }

    void CaseChecker::visit(common::Function &node) {
        expectedPatternCount = node.types.size() - 1;
        currentFunction = node.id;
        hasDefault = false;

        cout << "Case checker: " << to_string(node.cases.size()) << endl;       // DEBUG INFO
        cout << "Input Length: " << to_string(expectedPatternCount) << endl;    // DEBUG INFO

        if (node.cases.size() > 0) {
            for (auto c : node.cases) {
                c->accept(*this);
            }
        } else {
            std::cerr << "Error: No cases found @ " << currentFunction << " line " << to_string(node.line_no) << endl;
        }

        if(hasDefault == false) {
            cerr << "Error: No default case @ " << currentFunction << " line " << to_string(node.line_no) << " => should catch general case for " << to_string(node.types.size() -1) << " patterns" << endl;
        }
    }

    void CaseChecker::visit(common::Case &node) {
        if (node.patterns.size() != expectedPatternCount) {
            std::cerr << "Error: Invalid pattern  @ " << currentFunction << " line " << to_string(node.line_no);
            if (node.patterns.size() < expectedPatternCount) {
                std::cerr << " => too few patterns in case, expected " << to_string(expectedPatternCount) << " but got " << to_string(node.patterns.size()) << endl;
            } else {
                std::cerr << " => too many patterns in case, expected " << to_string(expectedPatternCount) << " but got " << to_string(node.patterns.size()) << endl;
            }
            return;
        } else {
            if (hasDefault == true) {
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