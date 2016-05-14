#include <iostream>
#include "CTree.h"

using namespace ctree;
using namespace std;

#define OutputT Output << getTaps(TapCount)

std::string getTaps(int TapCount) {
    std::string Res;

    for (int i = 0; i < TapCount; ++i) {
        Res += "    ";
    }

    return Res;
}

void Program::outputCode(std::ostream &Output, int TapCount = 0) {
    for (auto &Incl: Includes) {
        Incl->outputCode(Output, TapCount);
    }

    if (Includes.size())
        Output << endl;

    for (auto &Strct: Structs) {
        Strct->outputForward(Output, TapCount);
    }

    if (Structs.size())
        Output << endl;

    for (auto &Funcs: Functions) {
        Funcs->outputForward(Output, TapCount);
    }

    if (Functions.size())
        Output << endl;

    for (auto &Strct: Structs) {
        Strct->outputCode(Output, TapCount);
    }

    if (Structs.size())
        Output << endl;

    for (auto &Global: Globals) {
        Global->outputCode(Output, TapCount);
    }

    if (Globals.size())
        Output << endl;

    for (auto &Funcs: Functions) {
        Funcs->outputCode(Output, TapCount);
    }
}

void Include::outputCode(std::ostream &Output, int TapCount) {
    OutputT << "#include \"" << Path << "\"" << endl;
}

void Block::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << "{" << endl;

    for (auto &Stmt: Stmts) {
        Stmt->outputCode(Output, TapCount + 1);
    }

    OutputT << "}" << endl;
}

void Function::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << ReturnType << " " << Id << "(";

    for (size_t i = 0; i < Args.size(); ++i) {
        Output << Args[i].first << " " << Args[i].second;

        if (i != Args.size() - 1) {
            Output << ", ";
        }
    }

    Output << ")" << endl;

    Blk->outputCode(Output, TapCount);
    Output << endl;
}

void Function::outputForward(std::ostream &Output, int TapCount) {
    OutputT << ReturnType << " " << Id << "(";

    for (size_t i = 0; i < Args.size(); ++i) {
        Output << Args[i].first;

        if (i != Args.size() - 1) {
            Output << ", ";
        }
    }

    Output << ");" << endl;
}


void Struct::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << "struct " << Id << endl;
    OutputT << "{" << endl;

    TapCount++;

    for (auto &Field: Fields) {
        OutputT << Field.first << " " << Field.second << ";" << endl;
    }

    TapCount--;

    OutputT << "};" << endl;
    Output << endl;
}

void Struct::outputForward(std::ostream &Output, int TapCount) {
    OutputT << "typedef struct " << Id << " " << Id << ";" << endl;
}


void Decl::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << Type << " ";

    Expr->outputCode(Output, TapCount);
    Output << ";" << endl;
}

void IfElse::outputCode(std::ostream &Output, int TapCount = 0) {
    if (IsElse) {
        OutputT << "else ";
    } else {
        OutputT;
    }

    if (Expr) {
        OutputT << "if (";
        Expr->outputCode(Output, TapCount);
        Output << ")" << endl;
    } else if (IsElse) {
        Output << endl;
    }

    Blk->outputCode(Output, TapCount);
}

void For::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << "for (";

    if (Expr1) {
        Expr1->outputCode(Output, TapCount);
    }

    Output << "; ";

    if (Expr2) {
        Expr2->outputCode(Output, TapCount);
    }

    Output << "; ";

    if (Expr3) {
        Expr3->outputCode(Output, TapCount);
    }

    Output << ")" << endl;

    Blk->outputCode(Output, TapCount);
}

void While::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << "while (";

    if (Expr) {
        Expr->outputCode(Output, TapCount);
    }

    Output << ")" << endl;

    Blk->outputCode(Output, TapCount);
}

void Return::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << "return ";
    Expr->outputCode(Output, TapCount);

    Output << ";" << endl;
}

void Goto::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << "goto " << Label << ";" << endl;
}

void Label::outputCode(std::ostream &Output, int TapCount = 0) {
    OutputT << Id << ":" << endl;
}

void BinOp::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << "(";
    Left->outputCode(Output, TapCount);
    Output << ") " << Op << " (";
    Right->outputCode(Output, TapCount);
    Output << ")";
}

void UnOp::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << Op << " (";
    Child->outputCode(Output, TapCount);
    Output << ")";
}

void Call::outputCode(std::ostream &Output, int TapCount = 0) {
    Callee->outputCode(Output, TapCount);
    Output << "(";

    for (size_t i = 0; i < Args.size(); ++i) {
        Args[i]->outputCode(Output, TapCount);

        if (i != Args.size() - 1) {
            Output << ", ";
        }
    }

    Output << ")";
}

void Int::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << Value;
}

void Float::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << Value;
}

void Char::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << '\'' << Value << '\'';
}

void String::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << '"' << Value << '"';
}

void ArrayLiteral::outputCode(std::ostream &Output, int TapCount = 0) {
    Output << "{ ";

    for (size_t i = 0; i < Exprs.size(); ++i) {
        Exprs[i]->outputCode(Output, TapCount);

        if (i != Exprs.size() - 1) {
            Output << ", ";
        }
    }

    Output << " }";
}

void Ident::outputCode(std::ostream &Output, int TapCount) {
    Output << Value;
}


void ExprStmt::outputCode(std::ostream &Output, int TapCount) {
    OutputT;
    Expr->outputCode(Output, TapCount);
    Output << ";" << endl;
}

