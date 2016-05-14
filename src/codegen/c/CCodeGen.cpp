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

    /* The main function of the C program will be build below.
     * For readablity, the code that the C tree will generate,
     * can be seen below.
     *
     * int main(int argc, char** argv)
     * {
     *     strlist args = gcreate_strlist(0);
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
    MainBlock->Stmts.push_back(new Decl("int", new Ident("i")));

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

}

void CCodeGen::visit(common::AlgebraicDT &Node) {

}

void CCodeGen::visit(common::Product &Node) {

}

void CCodeGen::visit(common::Case &Node) {

}

void CCodeGen::visit(common::LambdaArg &Node) {

}

void CCodeGen::visit(common::Or &Node) {

}

void CCodeGen::visit(common::And &Node) {

}

void CCodeGen::visit(common::Equal &Node) {

}

void CCodeGen::visit(common::NotEqual &Node) {

}

void CCodeGen::visit(common::Lesser &Node) {

}

void CCodeGen::visit(common::Greater &Node) {

}

void CCodeGen::visit(common::LesserEq &Node) {

}

void CCodeGen::visit(common::GreaterEq &Node) {

}

void CCodeGen::visit(common::Add &Node) {

}

void CCodeGen::visit(common::Sub &Node) {

}

void CCodeGen::visit(common::Mul &Node) {

}

void CCodeGen::visit(common::Div &Node) {

}

void CCodeGen::visit(common::Mod &Node) {

}

void CCodeGen::visit(common::ListAdd &Node) {

}

void CCodeGen::visit(common::ProducerConsumer &Node) {

}

void CCodeGen::visit(common::Concat &Node) {

}

void CCodeGen::visit(common::To &Node) {

}

void CCodeGen::visit(common::UnPrint &Node) {

}

void CCodeGen::visit(common::ParExpr &Node) {

}

void CCodeGen::visit(common::Not &Node) {

}

void CCodeGen::visit(common::Negative &Node) {

}

void CCodeGen::visit(common::DoExpr &Node) {

}

void CCodeGen::visit(common::Assosiate &Node) {

}

void CCodeGen::visit(common::LambdaFunction &Node) {

}

void CCodeGen::visit(common::IdPattern &Node) {

}

void CCodeGen::visit(common::IntPattern &Node) {

}

void CCodeGen::visit(common::FloatPattern &Node) {

}

void CCodeGen::visit(common::CharPattern &Node) {

}

void CCodeGen::visit(common::BoolPattern &Node) {

}

void CCodeGen::visit(common::StringPattern &Node) {

}

void CCodeGen::visit(common::ListPattern &Node) {

}

void CCodeGen::visit(common::TuplePattern &Node) {

}

void CCodeGen::visit(common::ListSplit &Node) {

}

void CCodeGen::visit(common::WildPattern &Node) {

}

void CCodeGen::visit(common::AlgebraicPattern &Node) {

}

void CCodeGen::visit(common::ParPattern &Node) {

}

void CCodeGen::visit(common::IdExpr &Node) {

}

void CCodeGen::visit(common::IntExpr &Node) {

}

void CCodeGen::visit(common::FloatExpr &Node) {

}

void CCodeGen::visit(common::CharExpr &Node) {

}

void CCodeGen::visit(common::BoolExpr &Node) {

}

void CCodeGen::visit(common::StringExpr &Node) {

}

void CCodeGen::visit(common::ListExpr &Node) {

}

void CCodeGen::visit(common::TupleExpr &Node) {

}

void CCodeGen::visit(common::CallExpr &Node) {

}

void CCodeGen::visit(common::AlgebraicExpr &Node) {

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
     *     childtype value;
     *     int length;
     * };
     * */
    auto ListStruct = new Struct(Name);
    ListStruct->Fields.push_back({ Type, GNext });
    ListStruct->Fields.push_back({ GInt, GLength });
    CProg->Structs.push_back(ListStruct);

    /* list* gcrt_list(int count, ...)
     * {
     *     int i;
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
    CProg->Functions.push_back(Func);

    // int i;
    // va_list args;
    FuncBlock->Stmts.push_back(new Decl("int", new Ident("i")));
    FuncBlock->Stmts.push_back(new Decl("va_list", new Ident("args")));

    // list* res = malloc(sizeof(list));
    auto SizeOfRes = new Call(new Ident("sizeof"));
    SizeOfRes->Args.push_back(new Ident(Name));
    auto AllocRes = new Call(new Ident(GAlloc));
    AllocRes->Args.push_back(SizeOfRes);

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
    auto GetNext = new BinOp("->", new Ident("res"), new Ident(GNext));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetValue, new Ident("l"))));

    // res->length = l->length + 1;
    auto GetLengthRes = new BinOp("->", new Ident("res"), new Ident(GNext));
    auto GetLengthL = new BinOp("+", new BinOp("->", new Ident("l"), new Ident(GNext)), new Int(1));
    FuncBlock->Stmts.push_back(new ExprStmt(new BinOp("=", GetLengthRes, GetLengthL)));

    // return res;
    FuncBlock->Stmts.push_back(new Return(new Ident("res")));

    //----------------------------------------------





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

void CCodeGen::generateStd(Type &Ty) {

}

