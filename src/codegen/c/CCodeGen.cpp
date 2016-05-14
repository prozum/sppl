#include "CCodeGen.h"

using namespace codegen;
using namespace std;
using namespace common;
using namespace ctree;

CCodeGen::CCodeGen(parser::Driver &Drv)
        : CodeGenerator(Drv) {

    ListOffsets = vector<int>({0});
    FakeString = Type(TypeId::STRING, vector<Type>({ Type(TypeId::CHAR) }));
    StringList = Type(TypeId::LIST, vector<Type>({ FakeString }));
    RealString = Type(TypeId::LIST, vector<Type>({ Type(TypeId::CHAR) }));
}

void CCodeGen::visit(common::Program &Node) {
    Prog = &Node;
    CProg = new ctree::Program();

    generateStd();

    /* The main function of the C program will be build below.
     * For readablity, the code that the C tree will generate,
     * can be seen below.
     *
     * int main(int argc, char** argv)
     * {
     *     strlist* args = gcreate_strlist(0);
     *     int i;
     *
     *     for (i = argc - 1; i >= 0; i--)
     *     {
     *         args = gadd_strlist(args, gctr_str(argv[i]));
     *     }
     *
     *     umain(args);
     *     return 0;
     */

    string StrListName = getName(StringList);
    string StrListType = StrListName + typePostfix(StringList);

    auto MainBlock = new Block();

    // int main(int argc, char** argv)
    auto MainFunc = new ctree::Function("int", "main", MainBlock);
    MainFunc->Args.push_back({ "int", "argc" });
    MainFunc->Args.push_back({ "char**", "argv" });
    CProg->Functions.push_back(MainFunc);

    // strlist args = gcreate_strlist(0);
    auto StrListCreate = new Call(new Ident(GCreate + StrListName));
    StrListCreate->Args.push_back(new Int(0));
    MainBlock->Stmts.push_back(new Decl(StrListType, new BinOp("=", new Ident("args"), StrListCreate)));

    // int i;
    MainBlock->Stmts.push_back(new Decl(GInt, new Ident("i")));

    // for (i = argc - 1; i >= 0; --i)
    auto Expr1 = new BinOp("=", new Ident("i"), new BinOp("-", new Ident("argc"), new Int(1)));
    auto Expr2 = new BinOp(">=", new Ident("i"), new Int(0));
    auto Expr3 = new UnOp("--", new Ident("i"));
    auto ForBlock = new Block();
    MainBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // args = gadd_strlist(args, gctr_str(argv[i]));
    auto StrCreate = new Call(new Ident(GCreate + GString));
    StrCreate->Args.push_back(new Ident("i"));
    auto AddFunc = new Call(new Ident(GAdd + StrListName));
    AddFunc->Args.push_back(new Ident("args"));
    AddFunc->Args.push_back(StrCreate);
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("args"), AddFunc)));

    // umain(args);
    auto UMainCall = new Call(new Ident(GUser + "main"));
    UMainCall->Args.push_back(new Ident("args"));
    MainBlock->Stmts.push_back(new ExprStmt(UMainCall));

    // return 0;
    MainBlock->Stmts.push_back(new Return(new Int(0)));

    for (auto &Func : Node.Decls) {
        Func->accept(*this);
    }

    CProg->outputCode(*Drv.Out, 0);
}

void CCodeGen::visit(common::Function &Node) {
    auto& SubTypes = Node.Signature.Subtypes;
    string RetType = getName(SubTypes.back()) + typePostfix(SubTypes.back());

    CurrFunc = &Node;

    CurrBlock = new Block();
    auto Func = new ctree::Function(RetType, GUser + Node.Id, CurrBlock);
    CProg->Functions.push_back(Func);

    for (size_t i = 0; i < SubTypes.size() - 1; ++i) {
        Func->Args.push_back({ getName(SubTypes[i]) + typePostfix(SubTypes[i]), GArg + to_string(i) });
    }

    CurrBlock->Stmts.push_back(new Label("Entry"));

    for (auto &Case: Node.Cases) {
        Case->accept(*this);
    }

    auto Print = new Call(new Ident("printf"));
    Print->Args.push_back(new String("No cases realized in " + Node.Id + "\n"));
    CurrBlock->Stmts.push_back(new ExprStmt(Print));

    auto Exit = new Call(new Ident("exit"));
    Exit->Args.push_back(new Int(0));
    CurrBlock->Stmts.push_back(new ExprStmt(Exit));
}

void CCodeGen::visit(common::Case &Node) {
    CurrBlock = new Block(CurrBlock);


    vector<ctree::Expression*> Patterns;
    if (Node.Patterns.size() > 1) {
        for (size_t i = 0; i < Node.Patterns.size(); ++i) {
            PatternBuilder = new Ident(GArg + to_string(i));

            Node.Patterns[i]->accept(*this);

            if (LastExpr) {
                Patterns.push_back(LastExpr);
            }

            delete PatternBuilder;
        }
    }

    if (Patterns.size() == 0) {
        CurrBlock->Parent->Stmts.push_back(CurrBlock);
    } else if (Patterns.size() == 1) {
        auto If = new IfElse(Patterns.back(), CurrBlock);
        CurrBlock->Parent->Stmts.push_back(If);
    } else {
        auto Top = new BinOp("&&", nullptr, nullptr);
        auto CurrAnd = Top;

        for (size_t i = 0; i < Patterns.size(); ++i) {
            if (i == 0) {
                CurrAnd->Left = unique_ptr<ctree::Expression>(Patterns[i]);
            } else if (i < Patterns.size() - 1) {
                auto NewAnd = new BinOp("&&", Patterns[i], nullptr);
                CurrAnd->Right = unique_ptr<ctree::Expression>(static_cast<ctree::Expression*>(NewAnd));

                CurrAnd = NewAnd;
            } else {
                CurrAnd->Right = unique_ptr<ctree::Expression>(Patterns[i]);
            }

            auto If = new IfElse(Top, CurrBlock);
            CurrBlock->Parent->Stmts.push_back(If);
        }
    }


    if (Node.When) {
        CurrBlock = new Block(CurrBlock);

        Node.When->accept(*this);
        auto If = new IfElse(LastExpr, CurrBlock);
        CurrBlock->Parent->Stmts.push_back(If);
    }

    if (Node.TailRec) {
        auto Call = static_cast<CallExpr*>(Node.Expr.get());

        for (size_t i = 0; i < CurrFunc->Signature.Subtypes.size(); ++i) {
            Call->Args[i]->accept(*this);

            auto Assign = new BinOp("=", new Ident(GArg + to_string(i)), LastExpr);
            CurrBlock->Stmts.push_back(new ExprStmt(Assign));
        }

        CurrBlock->Stmts.push_back(new Goto("Entry"));
    } else {
        string RetType = getName(CurrFunc->Signature.Subtypes.back()) +
                         typePostfix(CurrFunc->Signature.Subtypes.back());

        Node.Expr->accept(*this);
        auto Assign = new BinOp("=", new Ident(GRes), LastExpr);
        CurrBlock->Stmts.push_back(new Decl(RetType, Assign));

        CurrBlock->Stmts.push_back(new Return(new Ident(GRes)));
    }

    if (Node.When) {
        CurrBlock = CurrBlock->Parent;
    }

    CurrBlock = CurrBlock->Parent;
}

void CCodeGen::visit(common::AlgebraicDT &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::Product &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::LambdaArg &Node) {
    throw runtime_error("Not implemented!");
}

ctree::Expression* CCodeGen::visitBinOp(common::BinaryOp& Op, std::string OpStr) {
    ctree::Expression* Left;
    ctree::Expression* Right;

    Op.Left->accept(*this);
    Left = LastExpr;
    Op.Right->accept(*this);
    Right = LastExpr;

    return new BinOp(OpStr, Left, Right);
}

void CCodeGen::visit(common::Or &Node) {
    LastExpr = visitBinOp(Node, "||");
}

void CCodeGen::visit(common::And &Node) {
    LastExpr = visitBinOp(Node, "&&");
}

ctree::Expression* CCodeGen::visitEqual(common::Type &Ty, common::Expression &Left, common::Expression &Right) {
    switch (Ty.Id) {
        case TypeId::TUPLE:
        case TypeId::LIST:
        case TypeId::STRING: {
            string Name = getName(Left.RetTy);

            auto Call = new ctree::Call(new Ident(GCompare + Name));
            Left.accept(*this);
            Call->Args.push_back(LastExpr);
            Right.accept(*this);
            Call->Args.push_back(LastExpr);

            return Call;
        }
        default: {
            ctree::Expression* CLeft;
            ctree::Expression* CRight;

            Left.accept(*this);
            CLeft = LastExpr;
            Right.accept(*this);
            CRight = LastExpr;

            return new BinOp("==", CLeft, CRight);
        }
    }
}

void CCodeGen::visit(common::Equal &Node) {
    LastExpr = visitEqual(Node.Left->RetTy, *Node.Left, *Node.Right);
}

void CCodeGen::visit(common::NotEqual &Node) {
    LastExpr = new UnOp("!", visitEqual(Node.Left->RetTy, *Node.Left, *Node.Right));
}

void CCodeGen::visit(common::Lesser &Node) {
    LastExpr = visitBinOp(Node, "<");
}

void CCodeGen::visit(common::Greater &Node) {
    LastExpr = visitBinOp(Node, ">");
}

void CCodeGen::visit(common::LesserEq &Node) {
    LastExpr = visitBinOp(Node, "<=");
}

void CCodeGen::visit(common::GreaterEq &Node) {
    LastExpr = visitBinOp(Node, ">=");
}

void CCodeGen::visit(common::Add &Node) {
    LastExpr = visitBinOp(Node, "+");
}

void CCodeGen::visit(common::Sub &Node) {
    LastExpr = visitBinOp(Node, "-");
}

void CCodeGen::visit(common::Mul &Node) {
    LastExpr = visitBinOp(Node, "*");
}

void CCodeGen::visit(common::Div &Node) {
    LastExpr = visitBinOp(Node, "/");
}

void CCodeGen::visit(common::Mod &Node) {
    LastExpr = visitBinOp(Node, "%");
}

void CCodeGen::visit(common::ListAdd &Node) {
    auto Add = new Call(new Ident(GAdd + getName(Node.RetTy)));

    Node.Right->accept(*this);
    Add->Args.push_back(LastExpr);
    Node.Left->accept(*this);
    Add->Args.push_back(LastExpr);

    LastExpr = Add;
}

void CCodeGen::visit(common::ProducerConsumer &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::Concat &Node) {
    auto Concat = new Call(new Ident(GConcat + getName(Node.RetTy)));

    Node.Left->accept(*this);
    Concat->Args.push_back(LastExpr);
    Node.Right->accept(*this);
    Concat->Args.push_back(LastExpr);

    LastExpr = Concat;
}

void CCodeGen::visit(common::To &Node) {
    throw runtime_error("Not implemented!");
}


void CCodeGen::visit(common::UnPrint &Node) {
    Node.Child->accept(*this);

    auto Print = new Call(new Ident(GPrint + getName(Node.RetTy)));
    Print->Args.push_back(LastExpr);

    LastExpr = Print;
}

void CCodeGen::visit(common::ParExpr &Node) {
    Node.Child->accept(*this);
}

void CCodeGen::visit(common::Not &Node) {
    Node.Child->accept(*this);
    LastExpr = new UnOp("!", LastExpr);
}

void CCodeGen::visit(common::Negative &Node) {
    Node.Child->accept(*this);
    LastExpr = new UnOp("-", LastExpr);
}

void CCodeGen::visit(common::DoExpr &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::Assosiate &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::LambdaFunction &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::IdPattern &Node) {
    ctree::Expression* Right;

    if ((Node.RetTy.Id == TypeId::LIST ||
        Node.RetTy.Id == TypeId::STRING) &&
        ListOffsets.back() == 0) {
        auto Call = new ctree::Call(new Ident(GAt + getName(Node.RetTy)));
        Call->Args.push_back(PatternBuilder->clone());
        Call->Args.push_back(new Int(ListOffsets.back()));

        Right = Call;
    } else {
        Right = PatternBuilder->clone();
    }

    auto Assign = new BinOp("=", new Ident(GUser + Node.Val), Right);
    CurrBlock->Stmts.push_back(new ExprStmt(Assign));
    LastExpr = nullptr;
}

void CCodeGen::visit(common::IntPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Int(Node.Val));
}

void CCodeGen::visit(common::FloatPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Float(Node.Val));
}

void CCodeGen::visit(common::CharPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Char(Node.Val));
}

void CCodeGen::visit(common::BoolPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Int(Node.Val));
}

void CCodeGen::visit(common::StringPattern &Node) {
    auto Call = new ctree::Call(new Ident(GCompare + GString));
    Call->Args.push_back(PatternBuilder->clone());
    Call->Args.push_back(new String(Node.str()));
    Call->Args.push_back(new Int(ListOffsets.back()));

    LastExpr = Call;
}

void CCodeGen::visit(common::ListPattern &Node) {
    vector<ctree::Expression*> Patterns;
    string TypeName = getName(Node.RetTy);

    auto GetLength = new BinOp("->", PatternBuilder->clone(), new Ident(GLength));

    ctree::Expression* Left;
    if (ListOffsets.back() > 0) {
        Left = new BinOp("-", GetLength, new Int(ListOffsets.back()));
    } else {
        Left = GetLength;
    }

    Patterns.push_back(new BinOp("==", Left, new Int(Node.Patterns.size())));

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        auto OldPatternBuilder = PatternBuilder;
        Call* CallAt;

        if (ListOffsets.back() + i > 0) {
            CallAt = new Call(new Ident(GAt + TypeName));
            CallAt->Args.push_back(OldPatternBuilder);
            CallAt->Args.push_back(new Int(ListOffsets.back() + i));
            PatternBuilder = new BinOp("->", CallAt, new Ident(GValue));
        } else {
            PatternBuilder = new BinOp("->", OldPatternBuilder, new Ident(GValue));
        }

        Node.Patterns[i]->accept(*this);

        // Ensure that OldPatternBuilder is not deleted by the
        // deconstructor of PatternBuilder
        if (ListOffsets.back() + i > 0) {
            CallAt->Args.erase(CallAt->Args.begin());
        } else {
            static_cast<BinOp*>(PatternBuilder)->Left = nullptr;
        }

        delete PatternBuilder;
        PatternBuilder = OldPatternBuilder;

        if (LastExpr) {
            Patterns.push_back(LastExpr);
        }
    }

    if (Patterns.size() == 1) {
        LastExpr = Patterns.back();
    } else {
        auto Top = new BinOp("&&", nullptr, nullptr);
        auto CurrAnd = Top;

        for (size_t i = 0; i < Patterns.size(); ++i) {
            if (i == 0) {
                CurrAnd->Left = unique_ptr<ctree::Expression>(Patterns[i]);
            } else if (i < Patterns.size() - 1) {
                auto NewAnd = new BinOp("&&", Patterns[i], nullptr);
                CurrAnd->Right = unique_ptr<ctree::Expression>(static_cast<ctree::Expression*>(NewAnd));

                CurrAnd = NewAnd;
            } else {
                CurrAnd->Right = unique_ptr<ctree::Expression>(Patterns[i]);
            }
        }

        LastExpr = Top;
    }
}

void CCodeGen::visit(common::TuplePattern &Node) {
    vector<ctree::Expression*> Patterns;

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        auto OldPatternBuilder = PatternBuilder;
        PatternBuilder = new BinOp("->", PatternBuilder, new Ident(GItem + to_string(i)));

        Node.Patterns[i]->accept(*this);

        // Ensure that OldPatternBuilder is not deleted by the
        // deconstructor of PatternBuilder
        static_cast<BinOp*>(PatternBuilder)->Left = nullptr;

        delete PatternBuilder;
        PatternBuilder = OldPatternBuilder;

        if (LastExpr) {
            Patterns.push_back(LastExpr);
        }
    }

    if (Patterns.size() == 0) {
        LastExpr = nullptr;
    } else if (Patterns.size() == 1) {
        LastExpr = Patterns.back();
    } else {
        auto Top = new BinOp("&&", nullptr, nullptr);
        auto CurrAnd = Top;

        for (size_t i = 0; i < Patterns.size(); ++i) {
            if (i == 0) {
                CurrAnd->Left = unique_ptr<ctree::Expression>(Patterns[i]);
            } else if (i < Patterns.size() - 1) {
                auto NewAnd = new BinOp("&&", Patterns[i], nullptr);
                CurrAnd->Right = unique_ptr<ctree::Expression>(static_cast<ctree::Expression*>(NewAnd));

                CurrAnd = NewAnd;
            } else {
                CurrAnd->Right = unique_ptr<ctree::Expression>(Patterns[i]);
            }
        }

        LastExpr = Top;
    }
}

void CCodeGen::visit(common::ListSplit &Node) {
    vector<ctree::Expression*> Patterns;
    string TypeName = getName(Node.RetTy);
    auto OldPatternBuilder = PatternBuilder;
    Call* CallAt;

    if (ListOffsets.back() > 0) {
        CallAt = new Call(new Ident(GAt + TypeName));
        CallAt->Args.push_back(OldPatternBuilder);
        CallAt->Args.push_back(new Int(ListOffsets.back()));
        PatternBuilder = new BinOp("->", CallAt, new Ident(GValue));
    } else {
        PatternBuilder = new BinOp("->", OldPatternBuilder, new Ident(GValue));
    }

    Node.Left->accept(*this);

    // Ensure that OldPatternBuilder is not deleted by the
    // deconstructor of PatternBuilder
    if (ListOffsets.back() > 0) {
        CallAt->Args.erase(CallAt->Args.begin());
    } else {
        static_cast<BinOp*>(PatternBuilder)->Left = nullptr;
    }

    delete PatternBuilder;
    PatternBuilder = OldPatternBuilder;

    if (LastExpr) {
        Patterns.push_back(LastExpr);
    }

    ListOffsets.back()++;
    Node.Right->accept(*this);
    ListOffsets.back()--;

    if (LastExpr) {
        Patterns.push_back(LastExpr);
    }

    if (Patterns.size() == 2) {
        LastExpr = new BinOp("&&", Patterns[0], Patterns[1]);
    } else if (Patterns.size() == 1) {
        LastExpr = Patterns.back();
    } else {
        LastExpr = nullptr;
    }
}

void CCodeGen::visit(common::WildPattern &Node) {
    LastExpr = nullptr;
}

void CCodeGen::visit(common::AlgebraicPattern &Node) {
    throw runtime_error("Not implemented!");
}

void CCodeGen::visit(common::ParPattern &Node) {
    Node.Pat->accept(*this);
}

void CCodeGen::visit(common::IdExpr &Node) {
    LastExpr = new Ident(Node.Val);
}

void CCodeGen::visit(common::IntExpr &Node) {
    LastExpr = new Int(Node.Val);
}

void CCodeGen::visit(common::FloatExpr &Node) {
    LastExpr = new Float(Node.Val);
}

void CCodeGen::visit(common::CharExpr &Node) {
    LastExpr = new Char(Node.Val);
}

void CCodeGen::visit(common::BoolExpr &Node) {
    LastExpr = new Int(Node.Val);
}

void CCodeGen::visit(common::StringExpr &Node) {
    LastExpr = new String(Node.Val);
}

void CCodeGen::visit(common::ListExpr &Node) {
    auto Create = new Call(new Ident(GCreate + getName(Node.RetTy)));
    Create->Args.push_back(new Int(Node.Elements.size()));

    for (auto &Expr: Node.Elements) {
        Expr->accept(*this);
        Create->Args.push_back(LastExpr);
    }

    LastExpr = Create;
}

void CCodeGen::visit(common::TupleExpr &Node) {
    auto Create = new Call(new Ident(GCreate + getName(Node.RetTy)));

    for (auto &Expr: Node.Elements) {
        Expr->accept(*this);
        Create->Args.push_back(LastExpr);
    }

    LastExpr = Create;
}

void CCodeGen::visit(common::CallExpr &Node) {
    Node.Callee->accept(*this);
    auto Call = new ctree::Call(LastExpr);

    for (auto &Expr: Node.Args) {
        Expr->accept(*this);
        Call->Args.push_back(LastExpr);
    }

    LastExpr = Call;
}

void CCodeGen::visit(common::AlgebraicExpr &Node) {
    throw runtime_error("Not implemented!");
}

string CCodeGen::typePostfix(Type &Ty) {
    // Generate the correct c type, based on sppl's types
    switch (Ty.Id) {
        case TypeId::FLOAT:
        case TypeId::CHAR:
        case TypeId::INT:
        case TypeId::BOOL:
        case TypeId::TUPLE:
        case TypeId::SIGNATURE:
            return "";
        case TypeId::STRING:
        case TypeId::LIST:
            return "*";
        default:
            // This should never happen. If it does, the type checker made a
            // mistake, or none supported features are being used
            throw runtime_error("This should never happen!");
    }
}

std::string CCodeGen::getName(common::Type &Ty) {
    // Generate the correct c type, based on sppl's types
    switch (Ty.Id) {
        case TypeId::FLOAT:
            return GFloat;
        case TypeId::CHAR:
            return GChar;
        case TypeId::INT:
            return GInt;
        case TypeId::BOOL:
            return GBool;
            // For tuples, lists, signatures and strings, custom types will be generated
        case TypeId::STRING:
            return StringName;
        case TypeId::TUPLE:
        case TypeId::SIGNATURE:
        case TypeId::LIST:
            return getOrGen(Ty);
        default:
            // This should never happen. If it does, the type checker made a
            // mistake, or none supported features are being used
            throw runtime_error("This should never happen!");
    }
}

std::string CCodeGen::getOrGen(common::Type &Ty) {
    auto Got = GenTypes.find(Ty);

    if (Got != GenTypes.end()) {
        return Got->second;
    } else {
        switch (Ty.Id) {
            case TypeId::TUPLE:
                return generateTuple(Ty);
            case TypeId::SIGNATURE:
                return generateSignature(Ty);
            case TypeId::LIST:
                return generateList(Ty);
            default:
                // This should never happen. If it does, the type checker made a
                // mistake, or none supported features are being used
                throw runtime_error("This should never happen!");
        }
    }
}

string CCodeGen::generateList(Type &Ty) {
    string Name = GGenerated + GList + to_string(ListCount++);
    string Type = Name + "*";
    string ChildrenName = getName(Ty.Subtypes.front());
    string ChildrenType = ChildrenName + typePostfix(Ty);

    /* struct list
     * {
     *     list* next;
     *     int length;
     *     childtype value;
     * };
     * */
    auto ListStruct = new Struct(Name);
    ListStruct->Fields.push_back({ Type, GNext });
    ListStruct->Fields.push_back({ GInt, GLength });
    ListStruct->Fields.push_back({ ChildrenType, GValue });
    CProg->Structs.push_back(ListStruct);

    /* list* gcrt_list(int count, ...)
     * {
     *     va_list args;
     *     list* res = malloc(sizeof(list));
     *     res->length = 0;
     *     res->next = NULL;
     *
     *     va_start(args, count);
     *
     *     while (--count)
     *     {
     *         res = gadd_list(res, va_arg(args, childtype));
     *     }
     *
     *     va_end(args);
     *     return res;
     * }
     * */

    // list* gcrt_list(int count, ...)
    auto FuncBlock = new Block();
    auto Func = new ctree::Function(Type, GCreate + Name, FuncBlock);
    Func->Args.push_back({ "int", "count" });
    Func->Args.push_back({ "...", "" });
    CProg->Functions.push_back(Func);

    // va_list args;
    FuncBlock->Stmts.push_back(new Decl("va_list", new Ident("args")));

    // list* res = malloc(sizeof(list));
    auto SizeOfRes = new Call(new Ident("sizeof"));
    SizeOfRes->Args.push_back(new Ident(Name));
    auto AllocRes = new Call(new Ident(GAlloc));
    AllocRes->Args.push_back(SizeOfRes);
    FuncBlock->Stmts.push_back(new Decl(Type, new BinOp("=", new Ident("res"), AllocRes)));

    // res->length = 0;
    auto GetLengthRes = new BinOp("->", new Ident("res"), new Ident(GNext));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetLengthRes, new Int(0))));

    // res->next = NULL;
    auto GetNext = new BinOp("->", new Ident("res"), new Ident(GNext));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetNext, new Ident("NULL"))));

    // va_start(args, count);
    auto VaStart = new Call(new Ident("va_start"));
    VaStart->Args.push_back(new Ident("args"));
    VaStart->Args.push_back(new Ident("count"));
    FuncBlock->Stmts.push_back(new ExprStmt(VaStart));

    // while (--count)
    auto WhileBlock = new Block(FuncBlock);
    auto While = new ctree::While(new UnOp("--", new Ident("count")), WhileBlock);
    FuncBlock->Stmts.push_back(While);

    // res = gadd_list(res, va_arg(args, childtype));
    auto VaArg = new Call(new Ident("va_arg"));
    VaArg->Args.push_back(new Ident("args"));
    VaArg->Args.push_back(new Ident(ChildrenType));
    auto AddList = new Call(new Ident(GAdd + Name));
    AddList->Args.push_back(new Ident("res"));
    AddList->Args.push_back(VaArg);
    WhileBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("res"), AddList)));

    // va_end(args);
    auto VaEnd = new Call(new Ident("va_end"));
    VaEnd->Args.push_back(new Ident("args"));
    FuncBlock->Stmts.push_back(new ExprStmt(VaEnd));

    // return res;
    FuncBlock->Stmts.push_back(new Return(new Ident("res")));

    //----------------------------------------------

    /* list* gadd_list(list* l, childtype v)
     * {
     *     list* res = malloc(sizeof(list));
     *     res->value = v;
     *     res->next = l;
     *     res->length = v->length + 1;
     *     return res;
     * }
     * */

    // list* gadd_list(list* l, childtype v)
    FuncBlock = new Block();
    Func = new ctree::Function(Type, GAdd + Name, FuncBlock);
    Func->Args.push_back({ Type, "l" });
    Func->Args.push_back({ ChildrenType, "v" });
    CProg->Functions.push_back(Func);

    // list* res = malloc(sizeof(list));
    SizeOfRes = new Call(new Ident("sizeof"));
    SizeOfRes->Args.push_back(new Ident(Name));
    AllocRes = new Call(new Ident(GAlloc));
    AllocRes->Args.push_back(SizeOfRes);
    FuncBlock->Stmts.push_back(new Decl(Type, new BinOp("=", new Ident("res"), AllocRes)));

    // res->value = v;
    auto GetValue = new BinOp("->", new Ident("res"), new Ident(GValue));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetValue, new Ident("v"))));

    // res->next = l;
    GetNext = new BinOp("->", new Ident("res"), new Ident(GNext));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetNext, new Ident("l"))));

    // res->length = l->length + 1;
    GetLengthRes = new BinOp("->", new Ident("res"), new Ident(GLength));
    auto GetLengthL = new BinOp("+", new BinOp("->", new Ident("l"), new Ident(GLength)), new Int(1));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetLengthRes, GetLengthL)));

    // return res;
    FuncBlock->Stmts.push_back(new Return(new Ident("res")));

    //----------------------------------------------

    /* list* gat_list(list* l, int i)
     * {
     *     if (l->length < i)
     *     {
     *         printf("Out of bound! list\n");
     *         exit(1);
     *     }
     *
     *     while (i-- > 0)
     *     {
     *         l = l->next;
     *     }
     *
     *     return l;
     * }
     * */

    // list* gat_list(list* l, int i)
    FuncBlock = new Block();
    Func = new ctree::Function(Type, GAt + Name, FuncBlock);
    Func->Args.push_back({ Type, "l" });
    Func->Args.push_back({ GInt, "i" });
    CProg->Functions.push_back(Func);

    // if (l->length < i)
    GetLengthL = new BinOp("->", new Ident("l"), new Ident(GLength));
    auto IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new BinOp("<", GetLengthL, new Ident("i")), IfBlock));

    // printf("Out of bound! list\n");
    auto Print = new Call(new Ident("printf"));
    Print->Args.push_back(new String("Out of bound! " + Name + "\\n"));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // exit(1);
    auto Exit = new Call(new Ident("exit"));
    Exit->Args.push_back(new Int(1));
    IfBlock->Stmts.push_back(new ExprStmt(Exit));

    // while (i-- > 0)
    WhileBlock = new Block(FuncBlock);
    While = new ctree::While(new BinOp(">", new UnOp("--", new Ident("i"), false), new Int(0)), WhileBlock);
    FuncBlock->Stmts.push_back(While);

    // l = l->next;
    auto LAssign = new BinOp("=", new Ident("l"), new BinOp("->", new Ident("l"), new Ident(GNext)));
    WhileBlock->Stmts.push_back(new ExprStmt(LAssign));

    // return l;
    FuncBlock->Stmts.push_back(new Return(new Ident("l")));

    //----------------------------------------------

    /* int gcompare_list(list* l1, list* l2)
     * {
     *     if (l1->length != l2->length)
     *     {
     *         return 0;
     *     }
     *
     *     while (l1->length)
     *     {
     *         if (l1->value != l2->value)
     *         {
     *             return 0;
     *         }
     *
     *         l1 = l1->next;
     *         l2 = l2->next;
     *     }
     *
     *     return 1;
     * }
     * */

    // int gcompare_list(list* l1, list* l2)
    FuncBlock = new Block();
    Func = new ctree::Function(GBool, GCompare + Name, FuncBlock);
    Func->Args.push_back({ Type, "l1" });
    Func->Args.push_back({ Type, "l2" });
    CProg->Functions.push_back(Func);

    // if (l1->length != l2->length)
    IfBlock = new Block(FuncBlock);
    auto L1Length = new BinOp("->", new Ident("l1"), new Ident(GLength));
    auto L2Length = new BinOp("->", new Ident("l2"), new Ident(GLength));
    auto L1CompL2 = new BinOp("!=", L1Length, L2Length);
    FuncBlock->Stmts.push_back(new IfElse(L1CompL2, IfBlock));

    // return 0;
    IfBlock->Stmts.push_back(new Return(new Int(0)));

    // while (l1->length)
    WhileBlock = new Block(FuncBlock);
    L1Length = new BinOp("->", new Ident("l1"), new Ident(GLength));
    FuncBlock->Stmts.push_back(new ctree::While(L1Length, WhileBlock));

    // if (l1->value != l2->value)
    IfBlock = new Block(WhileBlock);
    auto L1Value = new BinOp("->", new Ident("l1"), new Ident(GValue));
    auto L2Value = new BinOp("->", new Ident("l2"), new Ident(GValue));

    ctree::Expression* V1CompV2;
    switch (Ty.Subtypes.front().Id) {
        case TypeId::LIST:
        case TypeId::TUPLE: {
            auto CompCall = new Call(new Ident(GCompare + ChildrenName));
            CompCall->Args.push_back(L1Value);
            CompCall->Args.push_back(L2Value);
            V1CompV2 = CompCall;
            break;
        }
        default:
            V1CompV2 = new BinOp("!=", L1Value, L2Value);
            break;
    }

    FuncBlock->Stmts.push_back(new IfElse(V1CompV2, IfBlock));

    IfBlock->Stmts.push_back(new Return(new Int(0)));
    FuncBlock->Stmts.push_back(new Return(new Int(1)));

    //----------------------------------------------

    /*
     * */

    // TODO Concat

    //----------------------------------------------

    /*
     * */

    // TODO Print


    return Name;
}

string CCodeGen::generateSignature(Type &Ty) {
    // Result is needed, so we don't start generating something inside the signature
    stringstream Res;
    string Name = GGenerated + GSignature + to_string(SigCount++);

    return Name;
}

string CCodeGen::generateTuple(Type &Ty) {
    // Result is needed, so we don't start generating something inside the tuple
    stringstream Res;
    string Name = GGenerated + GTuple + to_string(TupleCount++);

    // ReturnType name of tuple generated
    return Name;
}

void CCodeGen::generateStd() {
    CProg->Includes.push_back(new Include("stdarg.h"));
    CProg->Includes.push_back(new Include("stdio.h"));
    CProg->Includes.push_back(new Include("stdlib.h"));
    CProg->Includes.push_back(new Include("string.h"));
    CProg->Includes.push_back(new Include("stdint.h"));
    CProg->Includes.push_back(new Include("inttypes.h"));

    if (GCed) {
        CProg->Includes.push_back(new Include("gc.h"));
    }

    StringName = generateList(RealString);
    StringType = StringName + typePostfix(RealString);
    GenTypes[FakeString] = StringName;
    generateList(StringList);


    //----------------------------------------------

    /* list* gcrt_string(char* v)
     * {
     *     int i;
     *     list* res = gcrt_list(0);
     *
     *     for (i = strlen(v) - 1; i >= 0; --i)
     *     {
     *         res = gadd_list(res, v[i]);
     *     }
     *
     *     return res;
     * }
     * */

    // list* gcrt_string(char* v)
    auto FuncBlock = new Block();
    auto Func = new ctree::Function(StringType, GCreate + GString, FuncBlock);
    Func->Args.push_back({ "char*", "v" });
    CProg->Functions.push_back(Func);

    // int i;
    // list* res = gcrt_list(0);
    FuncBlock->Stmts.push_back(new Decl(GInt, new Ident("i")));
    auto Create = new Call(new Ident(GCreate + StringType));
    Create->Args.push_back(new Int(0));
    FuncBlock->Stmts.push_back(new Decl(StringType, new BinOp("=", new Ident("res"), Create)));

    // for (i = strlen(v) - 1; i >= 0; --i)
    auto ForBlock = new Block(FuncBlock);
    auto StrLen = new Call(new Ident("strlen"));
    StrLen->Args.push_back(new Ident("v"));
    auto Expr1 = new BinOp("=", new Ident("i"), new BinOp("-", StrLen, new Int(1)));
    auto Expr2 = new BinOp(">=", new Ident("i"), new Int(0));
    auto Expr3 = new UnOp("--", new Ident("i"));
    FuncBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // res = gadd_list(res, v[i]);
    auto Add = new Call(new Ident(GAdd + StringName));
    Add->Args.push_back(new Ident("res"));
    Add->Args.push_back(new Ident("v[i]"));
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("res"), Add)));

    // return res;
    FuncBlock->Stmts.push_back(new Return(new Ident("res")));

    //----------------------------------------------

    /* int gcompare_string(list* l, char* v, int o)
     * {
     *     int i;
     *
     *     if (o)
     *     {
     *         l = gat_list(l, o);
     *     }
     *
     *     if (strlen(v) == l->length)
     *     {
     *         for (i = 0; l->length; ++i)
     *         {
     *             if (l->value != v[i])
     *             {
     *                 return 0;
     *             }
     *
     *             l = l->next;
     *         }
     *     }
     *     else
     *     {
     *         return 0;
     *     }
     *
     *     return 1;
     * }
     * */

    // int gcompare_string(list* l1, char* v, int offset)
    FuncBlock = new Block();
    Func = new ctree::Function(GBool, GCompare + GString, FuncBlock);
    Func->Args.push_back({ StringType, "l" });
    Func->Args.push_back({ "char*", "v" });
    Func->Args.push_back({ GInt, "o" });
    CProg->Functions.push_back(Func);

    // int i;
    // if (o)
    FuncBlock->Stmts.push_back(new Decl(GInt, new Ident("i")));
    auto IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new Ident("o"), IfBlock));

    // l = gat_list(l, o);
    auto At = new Call(new Ident(GAt + StringType));
    At->Args.push_back(new Ident("l"));
    At->Args.push_back(new Ident("o"));
    IfBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("l"), At)));

    // if (strlen(v) == l->length)
    StrLen = new Call(new Ident("strlen"));
    StrLen->Args.push_back(new Ident("v"));
    auto LLength = new BinOp("->", new Ident("l"), new Ident(GLength));
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new BinOp("==", StrLen, LLength), IfBlock));

    // for (i = 0; l->length; ++i)
    Expr1 = new BinOp("=", new Ident("i"), new Int(0));
    Expr2 = new BinOp("->", new Ident("l"), new Ident(GLength));
    Expr3 = new UnOp("++", new Ident("i"));
    ForBlock = new Block(IfBlock);
    IfBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // if (l->value != v[i])
    auto LValue = new BinOp("->", new Ident("l"), new Ident(GLength));
    IfBlock = new Block(ForBlock);
    ForBlock->Stmts.push_back(new IfElse(new BinOp("!=", LValue, new Ident("v[i]")), IfBlock));

    // return 0;
    IfBlock->Stmts.push_back(new Return(new Int(0)));

    // l = l->next;
    auto LNext = new BinOp("->", new Ident("l"), new Ident(GNext));
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("l"), LNext)));

    // else
    // {
    //     return 0;
    // }
    //
    // return 1;
    auto ElseBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(nullptr, ElseBlock, true));
    ElseBlock->Stmts.push_back(new Return(new Int(0)));
    FuncBlock->Stmts.push_back(new Return(new Int(1)));

    //----------------------------------------------

    /*
     * */

    // TODO Prints
}

