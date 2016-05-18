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
    CProg = new ctree::Program();

    generateStd();
    CProg->Directives.push_back(new Define("printfunc", "printf"));

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
     *     umain.call(args);
     *     return 0;
     */

    string StrListName = getName(StringList);
    string StrListType = getType(StringList);

    // int main(int argc, char** argv)
    auto MainBlock = new Block();
    auto MainFunc = new ctree::Function("int", "main", MainBlock);
    MainFunc->Args.push_back({ "int", "argc" });
    MainFunc->Args.push_back({ "char**", "argv" });
    CProg->Functions.push_back(MainFunc);

    // strlist args = gcreate_strlist(0);
    // int i;
    auto StrListCreate = new Call(new Ident(GCreate + StrListName));
    StrListCreate->Args.push_back(new Int(0));
    MainBlock->Stmts.push_back(new Decl(StrListType, new BinOp("=", new Ident("args"), StrListCreate)));
    MainBlock->Stmts.push_back(new Decl(GIntType, new Ident("i")));

    // for (i = argc - 1; i >= 0; --i)
    auto Expr1 = new BinOp("=", new Ident("i"), new BinOp("-", new Ident("argc"), new Int(1)));
    auto Expr2 = new BinOp(">=", new Ident("i"), new Int(0));
    auto Expr3 = new UnOp("--", new Ident("i"));
    auto ForBlock = new Block();
    MainBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // args = gadd_strlist(args, gctr_str(argv[i]));
    auto StrCreate = new Call(new Ident(GCreate + GString));
    StrCreate->Args.push_back(new UnOp("[i]", new Ident("argv"), false));
    auto AddFunc = new Call(new Ident(GAdd + StrListName));
    AddFunc->Args.push_back(new Ident("args"));
    AddFunc->Args.push_back(StrCreate);
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("args"), AddFunc)));

    // umain.call(args);
    auto UMainCall = new Call(new BinOp(".", new Ident(GUser + "main"), new Ident(GCall)));
    UMainCall->Args.push_back(new Ident("args"));
    MainBlock->Stmts.push_back(new ExprStmt(UMainCall));

    // return 0;
    MainBlock->Stmts.push_back(new Return(new Int(0)));

    // Visit Functions
    for (auto &Func : Node.Decls) {
        Func->accept(*this);
    }

    CProg->outputCode(*Drv.Out, 0);
}

void CCodeGen::visit(common::Function &Node) {
    auto& SubTypes = Node.Signature.Subtypes;
    string RetType = getType(SubTypes.back());

    CurrFunc = &Node;

    // sig ufuncname = { gfuncname };
    auto Val = new ArrayLiteral();
    Val->Exprs.push_back(new Ident(GFunc + Node.Id));
    CProg->Globals.push_back(new Decl(getType(Node.Signature), new BinOp("=", new Ident(GUser + Node.Id), Val)));

    // retype gfuncname(type_1 arg_1, type_2 arg_2, ... , type_n arg_n)
    CurrBlock = new Block();
    auto Func = new ctree::Function(RetType, GFunc + Node.Id, CurrBlock);
    CProg->Functions.push_back(Func);

    for (size_t i = 0; i < SubTypes.size() - 1; ++i) {
        Func->Args.push_back({ getType(SubTypes[i]), GArg + to_string(i) });
    }

    // Entry:
    CurrBlock->Stmts.push_back(new Label("Entry"));

    // Visit Cases
    for (auto &Case: Node.Cases) {
        Case->accept(*this);
    }

    // print("No cases realized in funcname\n");
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("No cases realized in " + Node.Id + "\\n"));
    CurrBlock->Stmts.push_back(new ExprStmt(Print));

    // exit(1);
    auto Exit = new Call(new Ident("exit"));
    Exit->Args.push_back(new Int(1));
    CurrBlock->Stmts.push_back(new ExprStmt(Exit));
}

void CCodeGen::visit(common::Case &Node) {
    vector<ctree::Expression*> Patterns;

    CurrBlock = new Block(CurrBlock);

    // Visit patterns and get all none null expressions
    // generated from visiting them.
    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        PatternBuilder = new Ident(GArg + to_string(i));

        Node.Patterns[i]->accept(*this);

        if (LastExpr) {
            Patterns.push_back(LastExpr);
        }

        delete PatternBuilder;
    }

    auto Compare = generateAndChain(Patterns);

    // if (pattern_1 && (pattern_2 && (... && pattern_n ...))
    if (Compare) {
        auto If = new IfElse(Compare, CurrBlock);
        CurrBlock->Parent->Stmts.push_back(If);
    } else {
        CurrBlock->Parent->Stmts.push_back(CurrBlock);
    }

    if (Node.When) {
        CurrBlock = new Block(CurrBlock);

        Node.When->accept(*this);

        // if (when_expr)
        auto If = new IfElse(LastExpr, CurrBlock);
        CurrBlock->Parent->Stmts.push_back(If);
    }

    if (Node.TailRec) {
        // We know, that if a case is tail recursive, then the expression
        // of that case is a call.
        auto Call = static_cast<CallExpr*>(Node.Expr.get());

        // Reset all the arguments to the arguments of the call.
        for (size_t i = 0; i < Call->Args.size(); ++i) {
            Call->Args[i]->accept(*this);

            // arg_i = callarg_i
            auto Assign = new BinOp("=", new Ident(GArg + to_string(i)), LastExpr);
            CurrBlock->Stmts.push_back(new ExprStmt(Assign));
        }

        // goto Entry;
        CurrBlock->Stmts.push_back(new Goto("Entry"));
    } else {
        string RetType = getType(CurrFunc->Signature.Subtypes.back());

        Node.Expr->accept(*this);

        // rettype res = expr;
        auto Assign = new BinOp("=", new Ident(GRes), LastExpr);
        CurrBlock->Stmts.push_back(new Decl(RetType, Assign));

        // We save the return value before returning, so that
        // some cleanup can happen here. As for now, no cleanup
        // is done.

        // return res;
        CurrBlock->Stmts.push_back(new Return(new Ident(GRes)));
    }

    // Reset back to the old CurrBlock
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

void CCodeGen::visit(common::Or &Node) {
    LastExpr = visitBinOp(Node, "||");
}

void CCodeGen::visit(common::And &Node) {
    LastExpr = visitBinOp(Node, "&&");
}

void CCodeGen::visit(common::Equal &Node) {
    LastExpr = visitEqual(Node);
}

void CCodeGen::visit(common::NotEqual &Node) {
    LastExpr = new UnOp("!", visitEqual(Node));
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
    // gadd_list(right, left)
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
    // gconcat_list(left, right)
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

    Call* Print;

    // print_type(child)
    if (Node.RetTy.Id == TypeId::STRING) {
        Print = new Call(new Ident(GPrint + GString));
    } else {
        Print = new Call(new Ident(GPrint + getName(Node.RetTy)));
    }

    Print->Args.push_back(LastExpr);

    if (Node.RetTy.Id == TypeId::STRING || Node.RetTy.Id == TypeId::CHAR){
        Print->Args.push_back(new Int(1));
    }

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
        ListOffsets.back() > 0) {

        auto Call = new ctree::Call(new Ident(GAt + getName(Node.RetTy)));
        Call->Args.push_back(PatternBuilder->clone());
        Call->Args.push_back(new Int(ListOffsets.back()));

        Right = Call;
    } else {
        Right = PatternBuilder->clone();
    }

    auto Assign = new BinOp("=", new Ident(GUser + Node.Val), Right);
    CurrBlock->Stmts.push_back(new Decl(getType(Node.RetTy), Assign));
    LastExpr = nullptr;
}

void CCodeGen::visit(common::IntPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Int(Node.Val));
}

void CCodeGen::visit(common::FloatPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Float(Node.Val));
}

void CCodeGen::visit(common::CharPattern &Node) {
    LastExpr = new BinOp("==", PatternBuilder->clone(), new Char("" + Node.Val));
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
            static_cast<BinOp*>(PatternBuilder)->Left.release();
        }

        delete PatternBuilder;
        PatternBuilder = OldPatternBuilder;

        if (LastExpr) {
            Patterns.push_back(LastExpr);
        }
    }

    LastExpr = generateAndChain(Patterns);
}

void CCodeGen::visit(common::TuplePattern &Node) {
    vector<ctree::Expression*> Patterns;

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        auto OldPatternBuilder = PatternBuilder;
        PatternBuilder = new BinOp("->", PatternBuilder, new Ident(GItem + to_string(i)));

        Node.Patterns[i]->accept(*this);

        // Ensure that OldPatternBuilder is not deleted by the
        // deconstructor of PatternBuilder
        static_cast<BinOp*>(PatternBuilder)->Left.release();

        delete PatternBuilder;
        PatternBuilder = OldPatternBuilder;

        if (LastExpr) {
            Patterns.push_back(LastExpr);
        }
    }

    LastExpr = generateAndChain(Patterns);
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
        static_cast<BinOp*>(PatternBuilder)->Left.release();
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
    LastExpr = new Ident(GUser + Node.Val);
}

void CCodeGen::visit(common::IntExpr &Node) {
    LastExpr = new Int(Node.Val);
}

void CCodeGen::visit(common::FloatExpr &Node) {
    LastExpr = new Float(Node.Val);
}

void CCodeGen::visit(common::CharExpr &Node) {
    char Chars[] = { Node.Val, '\0' };
    string s = string(Chars);

    LastExpr = new Char(s);
}

void CCodeGen::visit(common::BoolExpr &Node) {
    LastExpr = new Int(Node.Val);
}

void CCodeGen::visit(common::StringExpr &Node) {
    auto Create = new Call(new Ident(GCreate + GString));
    Create->Args.push_back(new String(Node.Val));

    LastExpr = Create;
}

void CCodeGen::visit(common::ListExpr &Node) {
    auto Create = new Call(new Ident(GCreate + getName(Node.RetTy)));
    Create->Args.push_back(new Int(Node.Elements.size()));

    for (size_t i = Node.Elements.size(); i > 0; --i) {
        Node.Elements[i - 1]->accept(*this);
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
    auto Call = new ctree::Call(new BinOp(".", LastExpr, new Ident(GCall)));

    for (auto &Expr: Node.Args) {
        Expr->accept(*this);
        Call->Args.push_back(LastExpr);
    }

    LastExpr = Call;
}

void CCodeGen::visit(common::AlgebraicExpr &Node) {
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

ctree::Expression* CCodeGen::visitEqual(common::BinaryOp& Equal) {
    auto &Ty = Equal.Left->RetTy;

    switch (Ty.Id) {
        case TypeId::TUPLE:
        case TypeId::LIST:
        case TypeId::STRING: {
            auto Call = new ctree::Call(new Ident(GCompare + getName(Ty)));
            Equal.Left->accept(*this);
            Call->Args.push_back(LastExpr);
            Equal.Right->accept(*this);
            Call->Args.push_back(LastExpr);

            return Call;
        }
        default: {
            ctree::Expression* CLeft;
            ctree::Expression* CRight;

            Equal.Left->accept(*this);
            CLeft = LastExpr;
            Equal.Right->accept(*this);
            CRight = LastExpr;

            return new BinOp("==", CLeft, CRight);
        }
    }
}

ctree::Expression* CCodeGen::generateAndChain(std::vector<ctree::Expression*>& Exprs) {
    if (Exprs.size() == 0) {
        return nullptr;
    } else if (Exprs.size() == 1) {
        return Exprs.back();
    } else {
        auto Top = new BinOp("&&", nullptr, nullptr);
        auto CurrAnd = Top;

        for (size_t i = 0; i < Exprs.size(); ++i) {
            if (i == 0) {
                CurrAnd->Left = unique_ptr<ctree::Expression>(Exprs[i]);
            } else if (i < Exprs.size() - 1) {
                auto NewAnd = new BinOp("&&", Exprs[i], nullptr);
                CurrAnd->Right = unique_ptr<ctree::Expression>(static_cast<ctree::Expression*>(NewAnd));

                CurrAnd = NewAnd;
            } else {
                CurrAnd->Right = unique_ptr<ctree::Expression>(Exprs[i]);
            }
        }

        return Top;
    }
}

std::string CCodeGen::getType(common::Type &Ty) {
    switch (Ty.Id) {
        case TypeId::FLOAT:
            return GFloatType;
        case TypeId::CHAR:
            return GCharType;
        case TypeId::INT:
            return GIntType;
        case TypeId::BOOL:
            return GBoolType;
        case TypeId::STRING:
            return StringType;
        case TypeId::TUPLE:
        case TypeId::LIST:
            return getOrGen(Ty) + "*";
        case TypeId::SIGNATURE:
            return getOrGen(Ty);
        default:
            // This should never happen. If it does, the type checker made a
            // mistake, or none supported features are being used
            throw runtime_error("This should never happen!");
    }
}

std::string CCodeGen::getName(common::Type &Ty) {
    switch (Ty.Id) {
        case TypeId::FLOAT:
            return GFloatName;
        case TypeId::CHAR:
            return GCharName;
        case TypeId::INT:
            return GIntName;
        case TypeId::BOOL:
            return GBoolName;
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
    auto &SubType = Ty.Subtypes.front();
    string Name = GList + to_string(ListCount++);
    string Type = Name + "*";
    string ChildrenName = getName(SubType);
    string ChildrenType = getType(SubType);

    GenTypes[Ty] = Name;

    /* struct list
     * {
     *     list* next;
     *     int length;
     *     childtype value;
     * };
     * */
    auto ListStruct = new Struct(Name);
    ListStruct->Fields.push_back({ Type, GNext });
    ListStruct->Fields.push_back({ GIntType, GLength });
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
     *     while (count--)
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
    Func->Args.push_back({ GIntType, "count" });
    Func->Args.push_back({ "...", "" });
    CProg->Functions.push_back(Func);

    // va_list args;
    FuncBlock->Stmts.push_back(new Decl("va_list", new Ident("args")));

    // list* res = malloc(sizeof(list));
    auto SizeOfRes = new Call(new Ident("sizeof"));
    SizeOfRes->Args.push_back(new Ident(Name));
    auto AllocRes = new Call(new Ident(Alloc));
    AllocRes->Args.push_back(SizeOfRes);
    FuncBlock->Stmts.push_back(new Decl(Type, new BinOp("=", new Ident("res"), AllocRes)));

    // res->length = 0;
    auto GetLengthRes = new BinOp("->", new Ident("res"), new Ident(GLength));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetLengthRes, new Int(0))));

    // res->next = NULL;
    auto GetNext = new BinOp("->", new Ident("res"), new Ident(GNext));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetNext, new Ident("NULL"))));

    // va_start(args, count);
    auto VaStart = new Call(new Ident("va_start"));
    VaStart->Args.push_back(new Ident("args"));
    VaStart->Args.push_back(new Ident("count"));
    FuncBlock->Stmts.push_back(new ExprStmt(VaStart));

    // while (count--)
    auto WhileBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new While(new UnOp("--", new Ident("count"), false), WhileBlock));

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
    AllocRes = new Call(new Ident(Alloc));
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
     *         print("Out of bound! list\n");
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
    Func->Args.push_back({ GIntType, "i" });
    CProg->Functions.push_back(Func);

    // if (l->length < i)
    GetLengthL = new BinOp("->", new Ident("l"), new Ident(GLength));
    auto IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new BinOp("<", GetLengthL, new Ident("i")), IfBlock));

    // print("Out of bound! list\n");
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("Out of bound! " + Name + "\\n"));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // exit(1);
    auto Exit = new Call(new Ident("exit"));
    Exit->Args.push_back(new Int(1));
    IfBlock->Stmts.push_back(new ExprStmt(Exit));

    // while (i-- > 0)
    WhileBlock = new Block(FuncBlock);
    auto GComp = new BinOp(">", new UnOp("--", new Ident("i"), false), new Int(0));
    FuncBlock->Stmts.push_back(new While(GComp, WhileBlock));

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
    Func = new ctree::Function(GBoolType, GCompare + Name, FuncBlock);
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
    FuncBlock->Stmts.push_back(new While(L1Length, WhileBlock));

    // if (l1->value != l2->value)
    IfBlock = new Block(WhileBlock);
    auto L1Value = new BinOp("->", new Ident("l1"), new Ident(GValue));
    auto L2Value = new BinOp("->", new Ident("l2"), new Ident(GValue));

    ctree::Expression* V1CompV2;
    switch (SubType.Id) {
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

    WhileBlock->Stmts.push_back(new IfElse(V1CompV2, IfBlock));

    // l1 = l1->next;
    // l2 = l2->next;
    auto L1Next = new BinOp("->", new Ident("l1"), new Ident(GNext));
    auto Assign = new BinOp("=", new Ident("l1"), L1Next);
    WhileBlock->Stmts.push_back(new ExprStmt(Assign));
    auto L2Next = new BinOp("->", new Ident("l2"), new Ident(GNext));
    Assign = new BinOp("=", new Ident("l2"), L2Next);
    WhileBlock->Stmts.push_back(new ExprStmt(Assign));

    IfBlock->Stmts.push_back(new Return(new Int(0)));
    FuncBlock->Stmts.push_back(new Return(new Int(1)));

    //----------------------------------------------

    /* list* gconcat_list(list* l1, list* l2)
     * {
     *     int i;
     *     list** elements = alloc(sizeof(list*) * l1->length);
     *
     *     for (i = 0; l1->length; ++i)
     *     {
     *         elements[i] = l1;
     *         l1 = l1->next;
     *     }
     *
     *     for (--i; i >= 0; --i)
     *     {
     *         l2 = gadd_list(l2, elements[i]->value);
     *     }
     *
     *     return l2;
     * }
     * */

    // list* gconcat_list(list* l1, list* l2)
    FuncBlock = new Block();
    Func = new ctree::Function(Type, GConcat + Name, FuncBlock);
    Func->Args.push_back({ Type, "l1" });
    Func->Args.push_back({ Type, "l2" });
    CProg->Functions.push_back(Func);

    // int i;
    // list** elements = alloc(sizeof(list*) * l1->length);
    FuncBlock->Stmts.push_back(new Decl(GIntType, new Ident("i")));
    auto SizeOf = new Call(new Ident("sizeof"));
    SizeOf->Args.push_back(new Ident(Type));
    L1Length = new BinOp("->", new Ident("l1"), new Ident(GLength));
    auto ElAlloc = new Call(new Ident(Alloc));
    ElAlloc->Args.push_back(new BinOp("*", SizeOf, L1Length));
    FuncBlock->Stmts.push_back(new Decl(Type + "*", new BinOp("=", new Ident("elements"), ElAlloc) ));

    // for (i = 0; l1->length; ++i)
    auto ForBlock = new Block(FuncBlock);
    ctree::Expression* Expr1 = new BinOp("=", new Ident("i"), new Int(0));
    ctree::Expression* Expr2 = new BinOp("->", new Ident("l1"), new Ident(GLength));
    ctree::Expression* Expr3 = new UnOp("++", new Ident("i"));
    FuncBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // elements[i] = l1;
    auto AtElI = new UnOp("[i]", new Ident("elements"), false);
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", AtElI, new Ident("l1"))));

    // l1 = l1->next;
    L1Next = new BinOp("->", new Ident("l1"), new Ident(GNext));
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("l1"), L1Next)));

    // for (--i; i >= 0; --i)
    ForBlock = new Block(FuncBlock);
    Expr1 = new UnOp("--", new Ident("i"));
    Expr2 = new BinOp(">=", new Ident("i"), new Int(0));
    Expr3 = new UnOp("--", new Ident("i"));
    FuncBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // l2 = gadd_list(l2, elements[i]->value);
    auto ElValue = new BinOp("->", new UnOp("[i]", new Ident("elements"), false), new Ident(GValue));
    auto Add = new Call(new Ident(GAdd +  Name));
    Add->Args.push_back(new Ident("l2"));
    Add->Args.push_back(ElValue);
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("l2"), Add)));

    // return l2;
    FuncBlock->Stmts.push_back(new Return(new Ident("l2")));

    //----------------------------------------------

    /* list* gprint_list(list* l)
     * {
     *     list* res = l;
     *     print("[");
     *
     *     while (l->length)
     *     {
     *         print_child(l->value);
     *         l = l->next;
     *
     *         if (l->length)
     *         {
     *             print(", ");
     *         }
     *     }
     *
     *     print("]");
     *     return res;
     * }
     * */

    // list* gprint_list(list* l)
    FuncBlock = new Block();
    Func = new ctree::Function(Type, GPrint + Name, FuncBlock);
    Func->Args.push_back({ Type, "l" });
    CProg->Functions.push_back(Func);

    // list* res = l;
    FuncBlock->Stmts.push_back(new Decl(Type, new BinOp("=", new Ident("res"), new Ident("l"))));

    // print("[");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("["));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));

    // while (l->length)
    WhileBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new While(new BinOp("->", new Ident("l"), new Ident(GLength)), WhileBlock));

    // print_child(l->value);
    if (SubType.Id == TypeId::STRING) {
        Print = new Call(new Ident(GPrint + GString));
    } else {
        Print = new Call(new Ident(GPrint + ChildrenName));
    }

    Print->Args.push_back(new BinOp("->", new Ident("l"), new Ident(GValue)));

    if (SubType.Id == TypeId::STRING || SubType.Id == TypeId::CHAR){
        Print->Args.push_back(new Int(0));
    }
    WhileBlock->Stmts.push_back(new ExprStmt(Print));

    // l = l->next;
    auto LNext = new BinOp("->", new Ident("l"), new Ident(GNext));
    WhileBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("l"), LNext)));

    // if (l->length)
    IfBlock = new Block(WhileBlock);
    auto LLength = new BinOp("->", new Ident("l"), new Ident(GLength));
    WhileBlock->Stmts.push_back(new IfElse(LLength , IfBlock));

    // print(", ");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String(", "));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // print("]");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("]"));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));

    // list* res = l;
    FuncBlock->Stmts.push_back(new Return(new Ident("res")));

    return Name;
}

string CCodeGen::generateSignature(Type &Ty) {
    auto &SubTypes = Ty.Subtypes;
    string Name = GSignature + to_string(SigCount++);
    string Type = Name;

    GenTypes[Ty] = Name;

    /* struct signature
     * {
     *     type_n (*call)(type_1, type_2, ... , type_n-1);
     * };
     * */

    string Args = "(";

    for (size_t i = 0; i < SubTypes.size() - 1; ++i) {
        Args += getType(SubTypes[i]);

        if (i < Ty.Subtypes.size() - 2)
            Args += ", ";
    }

    Args += ")";

    auto SigStrcut = new Struct(Name);
    SigStrcut->Fields.push_back({ getType(SubTypes.back()) + " (*" + GCall + ")", Args });
    CProg->Structs.push_back(SigStrcut);

    //----------------------------------------------

    /* signature gprint_signature(signature s, int is_top)
     * {
     *     print("(Type_1 -> Type_2 -> ... -> Type_n)");
     *     return s;
     * }
     * */

    // signature gprint_signature(signature s, int is_top)
    auto FuncBlock = new Block();
    auto Func = new ctree::Function(Type, GPrint + Name, FuncBlock);
    Func->Args.push_back({ Type, "s" });
    Func->Args.push_back({ GBoolType, "is_top" });
    CProg->Functions.push_back(Func);

    // "(Type_1 -> Type_2 -> ... -> Type_n)"
    string PrintString = "(";

    for (size_t i = 0; i < SubTypes.size(); ++i) {
        PrintString += SubTypes[i].str();

        if (i < SubTypes.size() - 1)
            PrintString += " -> ";
    }
    PrintString += ")";

    // print("(Type_1 -> Type_2 -> ... -> Type_n)");
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String(PrintString));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));

    // return s;
    FuncBlock->Stmts.push_back(new Return(new Ident("s")));

    return Name;
}

string CCodeGen::generateTuple(Type &Ty) {
    auto &SubTypes = Ty.Subtypes;
    string Name = GTuple + to_string(TupleCount++);
    string Type = Name + "*";

    GenTypes[Ty] = Name;

    /* struct tuple
     * {
     *     type_1 item_1;
     *     type_2 item_2;
     *     ...
     *     type_n item_n;
     * };
     * */

    auto TupleStruct = new Struct(Name);

    for (size_t i = 0; i < SubTypes.size(); ++i) {
        TupleStruct->Fields.push_back({ getType(SubTypes[i]), GItem + to_string(i) });
    }
    CProg->Structs.push_back(TupleStruct);

    //----------------------------------------------

    /* tuple* gcrt_tuple(type_1 item_1, type_2 item_2, ... , type_n item_n)
     * {
     *     tuple* res = alloc(sizeof(tuple));
     *     res->item_1 = item_1;
     *     res->item_2 = item_2;
     *     ...
     *     res->item_n = item_n;
     *
     *     return res;
     * }
     * */

    // tuple* gcrt_tuple(type_1 item_1, type_2 item_2, ... , type_n item_n)
    auto FuncBlock = new Block();
    auto Func = new ctree::Function(Type, GCreate + Name, FuncBlock);

    for (size_t i = 0; i < SubTypes.size(); ++i) {
        Func->Args.push_back({ getType(SubTypes[i]), GItem + to_string(i) });
    }
    CProg->Functions.push_back(Func);

    // tuple* res = alloc(sizeof(tuple));
    auto SizeOf = new Call(new Ident("sizeof"));
    SizeOf->Args.push_back(new Ident(Name));
    auto AllocTuple = new Call(new Ident(Alloc));
    AllocTuple->Args.push_back(SizeOf);
    FuncBlock->Stmts.push_back(new Decl(Type, new BinOp("=", new Ident("res"), AllocTuple)));

    /*     res->item_1 = item_1;
     *     res->item_2 = item_2;
     *     ...
     *     res->item_n = item_n;
     * */
    for (size_t i = 0; i < SubTypes.size(); ++i) {
        string ItemName = GItem + to_string(i);
        auto ResItem = new BinOp("->", new Ident("res"), new Ident(ItemName));
        FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", ResItem, new Ident(ItemName))));
    }

    // return res;
    FuncBlock->Stmts.push_back(new Return(new Ident("res")));

    //----------------------------------------------

    /* int gcompare_tuple(tuple* t1, tuple* t2)
     * {
     *     if (t1->item_1 != t2->item_1)
     *     {
     *         return 0;
     *     }
     *
     *     ...
     *
     *     if (t1->item_n != t2->item_n)
     *     {
     *         return 0;
     *     }
     *
     *     return 1;
     * }
     * */

    // int gcompare_tuple(tuple* t1, tuple* t2)
    FuncBlock = new Block();
    Func = new ctree::Function(GBoolType, GCompare + Name, FuncBlock);
    Func->Args.push_back({ Type, "t1" });
    Func->Args.push_back({ Type, "t2" });
    CProg->Functions.push_back(Func);

    for (size_t i = 0; i < SubTypes.size(); ++i) {
        // t1->item_i
        // t2->item_i
        string ItemName = GItem + to_string(i);
        auto T1Item = new BinOp("->", new Ident("t1"), new Ident(ItemName));
        auto T2Item = new BinOp("->", new Ident("t2"), new Ident(ItemName));

        ctree::Expression* Compare;

        switch (SubTypes[i].Id) {
            case TypeId::FLOAT:
            case TypeId::CHAR:
            case TypeId::INT:
            case TypeId::BOOL:
                Compare = new BinOp("!=", T1Item, T2Item);
                break;
            case TypeId::STRING:
            case TypeId::TUPLE:
            case TypeId::LIST: {
                auto CompCall = new Call(new Ident(GCompare + getName(SubTypes[i])));
                CompCall->Args.push_back(T1Item);
                CompCall->Args.push_back(T2Item);

                Compare = new UnOp("!", CompCall);
                break;
            }
            case TypeId::SIGNATURE:
                // Signature not supported
            default:
                // This should never happen. If it does, the type checker made a
                // mistake, or none supported features are being used
                throw runtime_error("This should never happen!");
        }

        // if (t1->item_n != t2->item_n)
        auto IfBlock = new Block(FuncBlock);
        FuncBlock->Stmts.push_back(new IfElse(Compare, IfBlock));

        // return 0;
        IfBlock->Stmts.push_back(new Return(new Int(0)));
    }

    // return 1;
    FuncBlock->Stmts.push_back(new Return(new Int(1)));

    //----------------------------------------------

    /* tuple* gprint_tuple(tuple* t)
     * {
     *     print("(");
     *     print_item_1(t->item_1);
     *     print(", ");
     *     print_item_2(t->item_2);
     *     print(", ");
     *     ...
     *     print(", ");
     *     print_item_n(t->item_n);
     *     print(")");
     *
     *     return t;
     * }
     * */

    // tuple* gprint_tuple(tuple* t)
    FuncBlock = new Block();
    Func = new ctree::Function(Type, GPrint + Name, FuncBlock);
    Func->Args.push_back({ Type, "t" });
    CProg->Functions.push_back(Func);

    // print("(");
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("("));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));

    /*
     *     print_item_1(t->item_1);
     *     print(", ");
     *     print_item_2(t->item_2);
     *     print(", ");
     *     ...
     *     print(", ");
     *     print_item_n(t->item_n);
     * */
    for (size_t i = 0; i < SubTypes.size(); ++i) {
        string ItemName = GItem + to_string(i);

        // print_item_i(t->item_i);
        if (SubTypes[i].Id == TypeId::STRING) {
            Print = new Call(new Ident(GPrint + GString));
        } else {
            Print = new Call(new Ident(GPrint + getName(SubTypes[i])));
        }

        Print->Args.push_back(new BinOp("->", new Ident("t"), new Ident(ItemName)));

        if (SubTypes[i].Id == TypeId::STRING || SubTypes[i].Id == TypeId::CHAR){
            Print->Args.push_back(new Int(0));
        }

        FuncBlock->Stmts.push_back(new ExprStmt(Print));

        // print(", ");
        if (i < SubTypes.size() - 1) {
            Print = new Call(new Ident(PrintFunc));
            Print->Args.push_back(new String(", "));
            FuncBlock->Stmts.push_back(new ExprStmt(Print));
        }
    }

    // print(")");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String(")"));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));
    FuncBlock->Stmts.push_back(new Return(new Ident("t")));

    // return t;
    return Name;
}

void CCodeGen::generateStd() {
    CProg->Directives.push_back(new Include("stdarg.h"));
    CProg->Directives.push_back(new Include("stdio.h"));
    CProg->Directives.push_back(new Include("stdlib.h"));
    CProg->Directives.push_back(new Include("string.h"));
    CProg->Directives.push_back(new Include("stdint.h"));
    CProg->Directives.push_back(new Include("inttypes.h"));

    StringName = getName(RealString);
    StringType = getType(RealString);
    GenTypes[FakeString] = StringName;
    GenTypes[Type(TypeId::CHAR)] = GCharType;
    GenTypes[Type(TypeId::BOOL)] = GBoolType;
    GenTypes[Type(TypeId::INT)] = GIntType;
    GenTypes[Type(TypeId::FLOAT)] = GFloatType;
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
    FuncBlock->Stmts.push_back(new Decl(GIntType, new Ident("i")));
    auto Create = new Call(new Ident(GCreate + StringName));
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
    Add->Args.push_back(new UnOp("[i]", new Ident("v"), false));
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
    Func = new ctree::Function(GBoolType, GCompare + GString, FuncBlock);
    Func->Args.push_back({ StringType, "l" });
    Func->Args.push_back({ "char*", "v" });
    Func->Args.push_back({ GIntType, "o" });
    CProg->Functions.push_back(Func);

    // int i;
    // if (o)
    FuncBlock->Stmts.push_back(new Decl(GIntType, new Ident("i")));
    auto IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new Ident("o"), IfBlock));

    // l = gat_list(l, o);
    auto At = new Call(new Ident(GAt + StringName));
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
    ForBlock->Stmts.push_back(new IfElse(new BinOp("!=", LValue, new UnOp("[i]", new Ident("v"), false)), IfBlock));

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

    /* int gprint_int(int i)
     * {
     *     print("%d", i);
     *     return i;
     * }
     * */

    // int gprint_int(int i)
    FuncBlock = new Block();
    Func = new ctree::Function(GIntType, GPrint + GIntName, FuncBlock);
    Func->Args.push_back({ GIntType, "i" });
    CProg->Functions.push_back(Func);

    // print("%d", i);
    // return i;
    auto Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new Ident("\"%\" PRId64 \"\""));
    Print->Args.push_back(new Ident("i"));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));
    FuncBlock->Stmts.push_back(new Return(new Ident("i")));

    //----------------------------------------------

    /* double gprint_float(double f)
     * {
     *     print("%lf", f);
     *     return f;
     * }
     * */

    // double gprint_float(double f)
    FuncBlock = new Block();
    Func = new ctree::Function(GFloatType, GPrint + GFloatName, FuncBlock);
    Func->Args.push_back({ GFloatType, "f" });
    CProg->Functions.push_back(Func);

    // print("%lf", f);
    // return value;
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("%lf"));
    Print->Args.push_back(new Ident("f"));
    FuncBlock->Stmts.push_back(new ExprStmt(Print));
    FuncBlock->Stmts.push_back(new Return(new Ident("f")));

    //----------------------------------------------

    /* int gprint_char(int c, int is_top)
     * {
     *     if (is_top)
     *     {
     *         print("%c", (char)c);
     *     }
     *     else
     *     {
     *         print("'%c'", (char)c);
     *     }
     *
     *     return c;
     * }
     * */

    // int gprint_char(int c, int is_top)
    FuncBlock = new Block();
    Func = new ctree::Function(GCharType, GPrint + GCharName, FuncBlock);
    Func->Args.push_back({ GCharType, "c" });
    Func->Args.push_back({ GBoolType, "is_top" });
    CProg->Functions.push_back(Func);

    // if (is_top)
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new Ident("is_top"), IfBlock));

    // print("%c", (char)c);
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("%c"));
    Print->Args.push_back(new UnOp("(char)", new Ident("c")));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // else
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(nullptr, IfBlock, true));

    // print("'%c'", (char)c);
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("'%c'"));
    Print->Args.push_back(new UnOp("(char)", new Ident("c")));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // return c;
    FuncBlock->Stmts.push_back(new Return(new Ident("c")));


    //----------------------------------------------

    /* int gprint_bool(int b)
     * {
     *     if (b)
     *     {
     *         print("True");
     *     }
     *     else
     *     {
     *         print("False");
     *     }
     *
     *     return value;
     * }
     * */

    // int gprint_bool(int b)
    FuncBlock = new Block();
    Func = new ctree::Function(GBoolType, GPrint + GBoolName, FuncBlock);
    Func->Args.push_back({ GBoolType, "b" });
    CProg->Functions.push_back(Func);

    // if (b)
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new Ident("b"), IfBlock));

    // print("True");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("True"));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // else
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(nullptr, IfBlock, true));

    // print("False");
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("False"));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // return value;
    FuncBlock->Stmts.push_back(new Return(new Ident("b")));

    //----------------------------------------------

    /* list* gprint_string(list* s, int is_top)
     * {
     *     char* buffer = alloc(sizeof(char) * s->length + 1);
     *     int i;
     *
     *     for (i = 0; s->length, ++i)
     *     {
     *         buffer[i] = (char)s->value;
     *         s = s->next;
     *     }
     *
     *     buffer[i] = '\0';
     *
     *     if (is_top)
     *     {
     *         print("%s", buffer);
     *     }
     *     else
     *     {
     *         print("\"%s\"", buffer);
     *     }
     *
     *     return s;
     * }
     * */

    // list* gprint_string(list* s, int is_top)
    FuncBlock = new Block();
    Func = new ctree::Function(StringType, GPrint + GString, FuncBlock);
    Func->Args.push_back({ StringType, "s" });
    Func->Args.push_back({ GBoolType, "is_top" });
    CProg->Functions.push_back(Func);

    // char* buffer = alloc(sizeof(char) * s->length + 1);
    // int i;
    auto SLength = new BinOp("+", new BinOp("->", new Ident("s"), new Ident(GLength)), new Int(1));
    auto SizeOf = new Call(new Ident("sizeof"));
    SizeOf->Args.push_back(new Ident("char"));
    auto Size = new BinOp("*", SizeOf, SLength);
    auto BufAlloc = new Call(new Ident(Alloc));
    BufAlloc->Args.push_back(Size);
    FuncBlock->Stmts.push_back(new Decl("char*", new BinOp("=", new Ident("buffer"), BufAlloc)));
    FuncBlock->Stmts.push_back(new Decl(GIntType, new Ident("i")));

    // for (i = 0; s->length, ++i)
    ForBlock = new Block(FuncBlock);
    Expr1 = new BinOp("=", new Ident("i"), new Int(0));
    Expr2 = new BinOp("->", new Ident("s"), new Ident(GLength));
    Expr3 = new UnOp("++", new Ident("i"));
    FuncBlock->Stmts.push_back(new For(Expr1, Expr2, Expr3, ForBlock));

    // buffer[i] = (char)s->value;
    auto SVal = new UnOp("(char)", new BinOp("->", new Ident("s"), new Ident(GValue)));
    auto AtBufI = new UnOp("[i]", new Ident("buffer"), false);
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", AtBufI, SVal)));

    // s = s->next;
    auto SNext = new BinOp("->", new Ident("s"), new Ident(GNext));
    ForBlock->Stmts.push_back(new ExprStmt(new BinOp("=", new Ident("s"), SNext)));

    // buffer[i] = '\0';
    AtBufI = new UnOp("[i]", new Ident("buffer"), false);
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", AtBufI, new Char("\\0"))));

    // if (is_top)
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(new Ident("is_top"), IfBlock));

    // print("%s", buffer);
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("%s"));
    Print->Args.push_back(new Ident("buffer"));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // else
    IfBlock = new Block(FuncBlock);
    FuncBlock->Stmts.push_back(new IfElse(nullptr, IfBlock, true));

    // print("\"%s\"", buffer);
    Print = new Call(new Ident(PrintFunc));
    Print->Args.push_back(new String("\\\"%s\\\""));
    Print->Args.push_back(new Ident("buffer"));
    IfBlock->Stmts.push_back(new ExprStmt(Print));

    // return s;
    FuncBlock->Stmts.push_back(new Return(new Ident("s")));
}

