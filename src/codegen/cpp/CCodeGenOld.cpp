#include "CCodeGenOld.h"

using namespace common;
using namespace std;
using namespace codegen;

CCodeGenOld::CCodeGenOld(parser::Driver &Drv)
    : CodeGenerator(Drv), Output(Drv.Out), Header(Drv.HOut),
      ListOffsets(vector<int>({0})),
      StringList(Type(TypeId::LIST, vector<Type>({ Type(TypeId::STRING, vector<Type>({ Type(TypeId::CHAR) })) }))),
      RealString(Type(TypeId::LIST, vector<Type>({ Type(TypeId::CHAR) }))),
      FakeString(Type(TypeId::STRING, vector<Type>({ Type(TypeId::CHAR) }))) { }

void CCodeGenOld::visit(Program &Node) {
    Prog = &Node;
    Function *Main = nullptr;

    *Output << "#define print printf" << endl;

    // Generate the standard functionality for every program
    generateStd();

    // Find the main function
    for (auto &Decl : Node.Decls) {
        if (typeid(*Decl.get()) == typeid(Function) &&
            ((Function *)Decl.get())->Id == "main") {
            Main = (Function *)Decl.get();
            break;
        }
    }

    // If no main exists the c code generater shouldn't compile
    if (!Main) {
        addError(Error("No main function"));
        return;
    }

    // Use getType on mains return type. This is done, so that the print function for
    // mains return type has been generated.
    getType(Main->Signature.Subtypes.back());

    string StrListName = getList(StringList);

    // Generate the c main function. This function should setup input and call sppls main.
    *Output << "int main(int argc, char** argv) { " << endl
            << "    " << StrListName << " *args = " << GGenerated << GCreate << StrListName << "(0); " << endl
            << "    int i; " << endl
            << endl
            << "    for(i = argc - 1; i >= 0; i--) { " << endl
            << "        args = " << GGenerated << GAdd << StrListName << "(args, " << GGenerated
                                 << GCreate << GString << "(argv[i])); " << endl
            << "    } " << endl
            <<  endl
            << "    " << GUser << GMain << "(args); " << endl
            << "    return 0; " << endl
            << "} " << endl
            << " " << endl;

    for (auto &Func : Node.Decls) {
        Func->accept(*this);
        *Output << endl;

        // We know that we are not generating something inside a function when we are here.
        // This means that we can safely output the generation of the types.
        outputBuffer();
    }
}

void CCodeGenOld::visit(Function &Node) {
    stringstream Func;
    string RetType = getType(Node.Signature.Subtypes.back());

    CurFunc = &Node;

    // Generate function name and return type
    Func << RetType << " " << GUser << Node.Id << "(";

    for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
        Func << getType(Node.Signature.Subtypes[i]) << " " << GGenerated + GArg + to_string(i);

        if (i != Node.Signature.Subtypes.size() - 2)
            Func << ", ";
    }

    Func << ")";

    *Header << Func.str() << "; \n " << endl;
    *Output << Func.str() << " { " << endl
            << "Start: " << endl
            << endl;

    for (auto &Case : Node.Cases) {
        Case->accept(*this);
        *Output << endl;

        Assignments.clear();
    }

    // If a case is not realised in a function, a runtime error should occure.
    // There should be some checks earlier in the compiler phase that prevents
    // this error though.
    *Output << "    print(\"No cases realized in " << CurFunc->Id << " \\n\"); " << endl
            << "    exit(1); " << endl
            << "} " << endl
            << endl;
}

void CCodeGenOld::visit(Case &Node) {
    stringstream Pattern;
    string ExtraTap = "   ";
    bool Empty = true;

    Pattern << "    if (";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Push the argument name assosiated with the current pattern on a stack.
        // This is done, so that patterns can be generated based on the argument.
        PatternBuilder.push_back(GGenerated + GArg + to_string(i));

        Node.Patterns[i]->accept(*this);

        PatternBuilder.pop_back();

        if (!LastPattern.empty()) {
            if (!Empty)
                Pattern << " && ";
            else
                Empty = false;

            Pattern << LastPattern;
        }
    }

    Pattern << ")";

    if (!Empty) {
        *Output << Pattern.str();
        ExtraTap = "";
    }

    *Output << ExtraTap << " {" << endl;

    // Assign all ids in the current case a value from the function arguments
    for (auto &Assign : Assignments) {
        *Output << "        " << Assign << endl;

        if (Assign == Assignments.back())
            *Output << endl;
    }

    // If the when expression exists, generate an if-statement for it
    if (Node.When) {

        // An expression stack is used when generating expressions, because other things could be generated
        // while an expression is being generated. This is mostly used when generating parallel code
        // but is kept here, since it could be usefull in the future
        ExprStack.push(stringstream());
        BeforeExpr.push_back(string());

        Node.When->accept(*this);

        outputBeforeExpr();

        *Output << endl
                << "        if (" << ExprStack.top().str() << ") {" << endl;

        ExprStack.pop();

        ExtraTap = "    ";
    }

    ExprStack.push(stringstream());
    BeforeExpr.push_back(string());

    if (Node.TailRec) {
        auto C = (CallExpr *)Node.Expr.get();
        for (size_t i = 0; i < CurFunc->Signature.Subtypes.size() - 1; ++i) {
            ExprStack.push(stringstream());

            C->Args[i]->accept(*this);

            outputBeforeExpr();

            // If the case is tail recursive, the arguments of the function is just assigned
            // to the argument of the recursive call
            *Output << endl
                    << "        " << ExtraTap << GGenerated << GArg << i << " = "
                                  << ExprStack.top().str() << "; " << endl;
            ExprStack.pop();
        }

        // When the arguments have been assigned, the program should then just jump
        // back to the start of the function
        *Output << "" << endl
                << "        goto Start; " << endl;
    } else {
        // Get the return types name, so the result of the function can be assigned to a variable
        string ResType = getType(CurFunc->Signature.Subtypes.back());
        ExprStack.push(stringstream());

        Node.Expr->accept(*this);

        outputBeforeExpr();

        // A res variable is being declared here, so that some cleanup can occure
        // before the function returns
        *Output << "        " << ExtraTap << ResType << " res = " << ExprStack.top().str() << "; " << endl;

        // No cleanup occures right now

        *Output << "        " << ExtraTap << "return res; " << endl;

        ExprStack.pop();
    }

    if (Node.When) {
        *Output << "        } " << endl;
    }

    *Output << "    } " << endl
            << endl;
}

void CCodeGenOld::visit(AlgebraicDT &Node) {
    addError(Error::NotImplemented(Node.Loc));
}

void CCodeGenOld::visit(Product &Node) {
    addError(Error::NotImplemented(Node.Loc));
}

string CCodeGenOld::getType(Type &Ty) {
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
    case TypeId::TUPLE:
        return getTuple(Ty);
    case TypeId::SIGNATURE:
        return getEnvironment(Ty);
    case TypeId::STRING:
        return StringTypeName + "*";
    case TypeId::LIST:
        return getList(Ty) + "*";
    default:
        // This should never happen. If it does, the type checker made a
        // mistake, or unsupported features might be in use
        assert(0 && "This should never happen!");
    }
}

string CCodeGenOld::generateList(Type &Ty) {
    string Name = GGenerated + GList + to_string(ListCount++);
    string ChildrenType = getType(Ty.Subtypes.front());

    // Generate the data type itself
    *Header << endl
            << "typedef struct "<< Name << " { " << endl
            << "    struct "<< Name << "* " << GNext << "; " << endl
            << "    " << ChildrenType << " " << GValue << "; " << endl
            << "    int " << GEmpty << "; " << endl
            << "    int " << GSize << "; " << endl
            << "} " << Name << "; " << endl
            << endl;

    // Generate add function
    *Header << Name << "* " << GGenerated << GAdd << Name << "(" << Name
                            << "* current, " << ChildrenType << " item); " << endl;
    Buffer << Name << "* " << GGenerated << GAdd << Name << "(" << Name
                           << "* current, " << ChildrenType << " item) { " << endl
           << "    " << Name << "* res = " << allocString() << "(sizeof(" << Name << "));" << endl
           << "    res->" << GValue << " = item; " << endl
           << "    res->" << GNext << " = current; " << endl
           << "    res->"  << GEmpty << " = 0; " << endl
           << "    res->" << GSize << " = current->" << GSize << " + 1; " << endl
           << "    return res; " << endl
           << "} " << endl
           << endl;

    // Generate create function for list
    *Header << Name << "* " << GGenerated << GCreate << Name << "(int count, ...); " << endl;
    Buffer << Name << "* " << GGenerated << GCreate << Name << "(int count, ...) { " << endl
           << "    int i; " << endl
           << "    va_list args; " << endl
           << "    " << Name << "* res = " << allocString() << "(sizeof(" << Name << ")); " << endl
           << "    res->"  << GEmpty << " = 1; " << endl
           << "    res->" << GSize << " = 0; " << endl
           << endl
           << "    va_start(args, count); " << endl
           << endl
           << "    for (i = 0; i < count; i++) { " << endl
           << "        res = " << GGenerated << GAdd << Name << "(res, va_arg(args, " << ChildrenType << ")); " << endl
           << "    } " << endl
           << endl
           << "    va_end(args); " << endl
           << "    return res; " << endl
           << "} " << endl
           << endl;

    // Generate at function for list
    *Header << Name << "* " << GGenerated << GAt << Name << "(" << Name << "* current, int index); " << endl;
    Buffer << Name << "* " << GGenerated << GAt << Name << "(" << Name << "* current, int index) { " << endl
           << "    if (current->" << GSize << " < index) { " << endl
           << "        print(\"Out of bound! " << Ty.str() << "\\n\"); " << endl
           << "        exit(1); " << endl
           << "    }" << endl
           << endl
           << "    while (index-- > 0) { " << endl
           << "        current = current->" << GNext << "; " << endl
           << "    } " << endl
           << endl
           << "    return current; " << endl
           << "} " << endl
           << endl;

    // Generate compare function for list
    *Header << "int " << GGenerated << GCompare << Name << "(" << Name << "* list1, " << Name << "* list2); " << endl;
    Buffer << "int " << GGenerated << GCompare << Name << "(" << Name << "* list1, " << Name << "* list2) { " << endl
           << "    if (list1->" << GSize << " != list2->" << GSize << ") { " << endl
           << "         return 0; " << endl
           << "    } " << endl
           << endl
           << "    while (!list1->" << GEmpty << ") {" << endl
           << "        if(";

    switch (Ty.Subtypes.front().Id) {
    case TypeId::LIST:
        Buffer << "!" << GGenerated << GCompare << getList(Ty.Subtypes.front())
                      << "(list1->" << GValue << ", list2->" << GValue << ")";
        break;
    case TypeId::TUPLE:
        Buffer << "!" << GGenerated << GCompare << getTuple(Ty.Subtypes.front())
                      << "(list1->" << GValue << ", list2->" << GValue << ")";
        break;
    default:
        Buffer << "list1->" << GValue << "!= list2->" << GValue;
        break;
    }

    Buffer << ") " << endl
           << "            return 0; " << endl
           << "        list1 = list1->" << GNext << ";" << endl
           << "        list2 = list2->" << GNext << ";" << endl
           << "    }" << endl
           << endl
           << "    return 1; " << endl
           << "} " << endl
           << endl;

    // Generation of concat
    *Header << Name << "* " << GGenerated << GConcat << Name << "("
                            << Name << "* list1, " << Name << "* list2); " << endl;
    Buffer << Name << "* " << GGenerated << GConcat << Name << "("
                           << Name << "* list1, " << Name << "* list2) { " << endl
           << "    int i; " << endl
           << "    " << Name << "** elements = " << allocString() << "(sizeof(" << Name << "*) * list1->" << GSize << "); " << endl
           << endl
           << "    for (i = 0; !list1->" << GEmpty << "; ++i) { " << endl
           << "        elements[i] = list1; " << endl
           << "        list1 = list1->" << GNext << "; " << endl
           << "    } " << endl
           << endl
           << "    for (--i; i >= 0; --i) { " << endl
           << "        list2 = " << GGenerated << GAdd << Name << "(list2, elements[i]->" << GValue << ");" << endl
           << "    } " << endl
           << endl
           << "    return list2; " << endl
           << "} " << endl
           << endl;

    // generation of tostring
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name << "* value); " << endl;
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name << "* value) { " << endl
           << "    "  << StringTypeName << "* comma = " << GGenerated << GCreate << GString << "(\", \"); " << endl
           << "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\"]\"); " << endl
           << "    int i; " << endl
           << endl
           << "    while (!value->" << GEmpty << ") { " << endl
           << "        res = " << GGenerated << GConcat << StringTypeName << "("
                               << ToStrings[Ty.Subtypes.front()] << "(value->" << GValue << "), res); " << endl
           << "        value = value->" << GNext << ";" << endl
           << endl
           << "        if (!value->" << GEmpty << ") " << endl
           << "            res = " << GGenerated << GConcat << StringTypeName << "(comma, res); " << endl
           << "    } " << endl
           << endl
           << "    res = " << GGenerated << GAdd << StringTypeName << "(res, '['); " << endl
           << "    return res; " << endl
           << "} " << endl
           << endl;

    // generate print
    Prints[Ty] = GGenerated + GPrint + Name;

    *Header << Name << "* " << Prints[Ty] << "(" << Name << "* value, int is_top); " << endl;
    Buffer << Name << "* " << Prints[Ty] << "(" << Name << "* value, int is_top) { " << endl
            << "    " << Name << "* res = value;" << endl
            << "    print(\"[\"); \n" << endl
            << "    while (!value->" << GEmpty << ") {" << endl
            << "        " << Prints[Ty.Subtypes.front()] << "(value->" << GValue << ", 0);" << endl
            << "        value = value->" << GNext << ";" << endl
            << "        if (!value->" << GEmpty << ")" << endl
            << "            print(\", \"); \n" << endl
            << "    }" << endl
            << "    print(\"]\"); \n" << endl
            << "    return res;" << endl
            << "} " << endl
            << endl;

    Lists[Ty] = Name;

    // ReturnType name of list generated
    return Name;
}

string CCodeGenOld::generateEnvironment(Type &Ty) {
    // Result is needed, so we don't start generating something inside the signature
    stringstream Res;
    string Name = GGenerated + GSignature + to_string(SigCount++);

    Res << "typedef " << getType(Ty.Subtypes.back()) << "(*" << Name << ")(";

    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        Res << getType(Ty.Subtypes[i]);

        if (i != Ty.Subtypes.size() - 2)
            Res << ", ";
    }
    
    Res << "); " << endl;

    *Header << Res.str();

    // Generation of signature to string
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name << " value); " << endl;
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name << " value) { " << endl
           << "    return " << GGenerated << GCreate << GString << "(\"" << Ty.str() << "\"); " << endl
           << "} " << endl
           << endl;

    // Generate print
    Prints[Ty] = GGenerated + GPrint + Name;

    *Header << Name << " " << Prints[Ty] << "(" << Name << " value, int is_top); " << endl;
    Buffer << Name << " " << Prints[Ty] << "(" << Name << " value, int is_top) { " << endl
           << "    print(\"" << Ty.str() << "\"); " << endl
           << "    return value;" << endl
           << "} " << endl
           << endl;

    Closures[Ty] = Name;
    return Name;
/*
    // Result is needed, so we don't generate something inside the signature,
    // while generating other types
    stringstream Res;
    string Name = GGenerated + GSignature + to_string(SigCount);

    // Increase signature count, so next signature doesn't have the same name
    SigCount++;

    Res << "typedef struct " << Name << " {" << endl
        << "    " << getType(Ty.Subtypes.back()) << " (* call)(struct " << Name
        << "*";

    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        Res << ", ";

        // Get the actual type of the argument
        Res << getType(Ty.Subtypes[i]);
    }

    Res << "); " << endl
           "} "
        << Name << ";\n" << endl;

    *Header << "" << endl << Res.str();

    // Generation of signature to string
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
            << " value); " << endl;
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
           << " value) { " << endl
              "    return "
           << GGenerated << GCreate << GString << "(\"" << Ty.str() << "\"); " << endl
                                                        << "} " << endl
                                                        << " " << endl;

    // Save signature in signature hash map
    Closures[Ty] = Name;

    // ReturnType name of signature generated
    return Name;
 */
}

string CCodeGenOld::generateTuple(Type &Ty) {
    // Result is needed, so we don't start generating something inside the tuple
    stringstream Res;
    string Name = GGenerated + GTuple + to_string(TupleCount++);

    // Generate the tuple struct
    Res << endl
        << "typedef struct " << Name << " {" << endl;

    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        Res << "    " << getType(Ty.Subtypes[i]) << " " << GItem << i << ";" << endl;
    }
    Res << "} " << Name << "; " << endl;

    *Header << Res.str();
    Res.str("");

    // Generate a create function for the tuple
    Res << Name << " " << GGenerated << GCreate << Name << "(";

    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        Res << getType(Ty.Subtypes[i]) << " " << GItem << i;

        if (i < Ty.Subtypes.size() - 1)
            Res << ", ";
    }

    Res << ")";

    *Header << Res.str() << "; " << endl;
    Buffer << Res.str() << " { " << endl
           << "    " << Name << " res; " << endl;

    // For each item in res, assign values
    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        Buffer << "    res." << GItem << i << " = " << GItem << i << "; " << endl;
    }

    Buffer << endl
           << "    return res; " << endl
           << "} " << endl
           << endl;

    // Generate a compare function for this tuple
    *Header << "int " << GGenerated << GCompare << Name << "(" << Name << " tuple1, " << Name << " tuple2); " << endl;
    Buffer << "int " << GGenerated << GCompare << Name << "(" << Name << " tuple1, " << Name << " tuple2) { " << endl;

    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        Buffer << "    if (";

        switch (Ty.Subtypes[i].Id) {
        case TypeId::STRING:
        case TypeId::LIST:
            Buffer << "!" << GGenerated << GCompare << Lists[Ty.Subtypes[i]] << "(tuple1."
                          << GItem << i << ", tuple2."
                          << GItem << i << ")";
            break;
        case TypeId::TUPLE:
            Buffer << "!" << GGenerated << GCompare << Tuples[Ty.Subtypes[i]] << "(tuple1."
                          << GItem << i << ", tuple2."
                          << GItem << i << ")";
            break;
        default:
            Buffer << "(tuple1." << GItem << i << " != tuple2."
                                 << GItem << i << ")";
            break;
        }

        Buffer << ") " << endl
               << "        return 0;" << endl;
    }

    Buffer << endl
           << "    return 1; " << endl
           << "} " << endl
           << endl;

    // Generate a to_string for the tuple
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name << " value); " << endl;
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name << " value) { " << endl
           << "    " << StringTypeName << "* comma = " << GGenerated << GCreate << GString << "(\", \"); " << endl
           << "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\")\"); " << endl
           << endl;

    for (size_t i = Ty.Subtypes.size() - 1; i != 0; --i) {
        Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "(" << ToStrings[Ty.Subtypes[i]]
                               << "(value." << GItem << i << ")" << ", res); " << endl
               << "    res = " << GGenerated << GConcat << StringTypeName << "(comma, res); " << endl;
    }

    Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "(" << ToStrings[Ty.Subtypes[0]]
                           << "(value." << GItem << "0), res); " << endl
           << "    res = " << GGenerated << GAdd << StringTypeName << "(res, '('); " << endl
           << endl
           << "    return res; " << endl
           << "} " << endl
           << endl;

    // Generate print for tuple
    Prints[Ty] = GGenerated + GPrint + Name;

    *Header << Name << " " << Prints[Ty] << "(" << Name << " value, int is_top); " << endl;
    Buffer << Name << " " << Prints[Ty] << "(" << Name << " value, int is_top) { " << endl
           << "    print(\"(\");" << endl;

    for (size_t i = 0; i < Ty.Subtypes.size(); i++) {
        Buffer << "    " << Prints[Ty.Subtypes[i]] << "(value." << GItem << i << ", 0);" << endl;

        if (i < Ty.Subtypes.size() - 1)
            Buffer << "    print(\", \");" << endl;
    }

    Buffer << "    print(\")\");" << endl
           << "    return value;" << endl
           << "} " << endl
           << endl;

    // Save tuple in tuple hash map
    Tuples[Ty] = Name;

    // ReturnType name of tuple generated
    return Name;
}

void CCodeGenOld::generateStd() {
    *Output << "#include \"test.h\" \n " << endl;

    *Header << "#include <stdarg.h> " << endl
            << "#include <stdio.h> " << endl
            << "#include <stdlib.h> " << endl
            << "#include <string.h> " << endl
            << "#include <stdint.h> " << endl
            << "#include <inttypes.h> " << endl;

    if (GC) {
        *Header << "#include <gc.h> " << endl;
    }

    ToStrings[Type(TypeId::INT)] = GGenerated + GToString + "int";
    ToStrings[Type(TypeId::BOOL)] = GGenerated + GToString + "bool";
    ToStrings[Type(TypeId::FLOAT)] = GGenerated + GToString + "float";
    ToStrings[Type(TypeId::CHAR)] = GGenerated + GToString + "char";
    ToStrings[FakeString] = GGenerated + GToString + "string";
    Prints[Type(TypeId::INT)] = GGenerated + GPrint + "int";
    Prints[Type(TypeId::BOOL)] = GGenerated + GPrint + "bool";
    Prints[Type(TypeId::FLOAT)] = GGenerated + GPrint + "float";
    Prints[Type(TypeId::CHAR)] = GGenerated + GPrint + "char";
    Prints[FakeString] = GGenerated + GPrint + "string";


    StringTypeName = GGenerated + GList + to_string(ListCount);
    Lists[FakeString] = StringTypeName;

    generateList(RealString);
    generateList(StringList);

    outputBuffer();

    // Generation of string constructer starts here
    *Header << StringTypeName << "* " << GGenerated << GCreate << GString << "(char* values);" << endl;
    *Output << StringTypeName << "* " << GGenerated << GCreate << GString << "(char* values) { " << endl
            << "    int i, str_length = strlen(values); " << endl
            << "    " << StringTypeName << "* res = " << GGenerated << GCreate << StringTypeName << "(0); " << endl
            << endl
            << "    for (i = str_length - 1; i >= 0; i--) { " << endl
            << "        res = " << GGenerated << GAdd << StringTypeName << "(res, values[i]); " << endl
            << "    } " << endl
            << endl
            << "    return res; " << endl
            << "} " << endl
            << endl;

    // Generation of string compare
    *Header << "int " << GGenerated << GCompare << GString << "(" << StringTypeName << "* string, char* values, int offset); " << endl;
    *Output << "int " << GGenerated << GCompare << GString << "(" << StringTypeName << "* string, char* values, int offset) { " << endl
            << "    int i, size = strlen(values); " << endl
            << endl
            << "    if (size == string->" << GSize << ") { " << endl
            << "        for (i = 0; i < size; i++) { " << endl
            << "            if (" << GGenerated << GAt << StringTypeName << "(string, i)->" << GValue << " != values[i]) " << endl
            << "                return 0; " << endl
            << "        } " << endl
            << "    } else { " << endl
            << "        return 0; " << endl
            << "    } " << endl
            << endl
            << "    return 1; " << endl
            << "} " << endl
            << endl;

    // Generation of default to_string methods
    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::INT)] << "(" << GInt << " value); " << endl;
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::INT)] << "(" << GInt << " value) { " << endl
            << "    char buffer[100]; " << endl
            << "    sprintf(buffer, \"%\" PRId64 \"\", value); " << endl
            << "    return " << GGenerated << GCreate << GString << "(buffer); " << endl
            << "}" << endl
            << endl;

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::BOOL)] << "(" << GBool << " value); " << endl;
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::BOOL)] << "(" << GBool << " value) { " << endl
            << "    if (value)" << endl
            << "        return " << GGenerated << GCreate << GString << "(\"True\");" << endl
            << "    else" << endl
            << "        return " << GGenerated << GCreate << GString << "(\"False\");" << endl
            << "}" << endl
            << endl;

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::FLOAT)] << "(" << GFloat << " value); " << endl;
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::FLOAT)] << "(" << GFloat << " value) { " << endl
            << "    char buffer[100]; " << endl
            << "    sprintf(buffer, \"%lf\", value); " << endl
            << "    return " << GGenerated << GCreate << GString << "(buffer); " << endl
            << "} " << endl
            << " " << endl;

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::CHAR)] << "(" << GChar << " value); " << endl;
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::CHAR)] << "(" << GChar << " value) { " << endl
            << "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\"\\'\"); " << endl
            << "    res = " << GGenerated << GAdd << StringTypeName << "(res, value); " << endl
            << "    res = " << GGenerated << GAdd << StringTypeName << "(res, '\\''); " << endl
            << "    return res; " << endl
            << "} " << endl
            << endl;

    *Header << StringTypeName << "* " << ToStrings[FakeString] << "(" << StringTypeName << "* value); " << endl;
    *Output << StringTypeName << "* " << ToStrings[FakeString] << "(" << StringTypeName << "* value) { " << endl
            << "    " << StringTypeName << "* res = " << GGenerated << GCreate << GString << "(\"\\\"\"); " << endl
            << "    res = " << GGenerated << GConcat << StringTypeName << "(value, res); " << endl
            << "    res = " << GGenerated << GAdd << StringTypeName << "(res, '\"'); " << endl
            << "} " << endl
            << endl;

    // Generate default prints
    *Header << GInt << " " << Prints[Type(TypeId::INT)] << "(" << GInt << " value, int is_top); " << endl;
    *Output << GInt << " " << Prints[Type(TypeId::INT)] << "(" << GInt << " value, int is_top) { " << endl
            << "    print(\"%\" PRId64 \"\", value);" << endl
            << "    return value;" << endl
            << "} " << endl
            << endl;

    *Header << GFloat << " " << Prints[Type(TypeId::FLOAT)] << "(" << GFloat << " value, int is_top); " << endl;
    *Output << GFloat << " " << Prints[Type(TypeId::FLOAT)] << "(" << GFloat << " value, int is_top) { " << endl
            << "    print(\"%lf\", value);" << endl
            << "    return value;" << endl
            << "} " << endl
            << endl;

    *Header << GChar << " " << Prints[Type(TypeId::CHAR)] << "(" << GChar << " value, int is_top); " << endl;
    *Output << GChar << " " << Prints[Type(TypeId::CHAR)] << "(" << GChar << " value, int is_top) { " << endl
            << "    if (is_top)" << endl
            << "        print(\"%c\", (char)value);" << endl
            << "    else" << endl
            << "        print(\"'%c'\", (char)value);" << endl
            << "    return value;" << endl
            << "} " << endl
            << endl;

    *Header << GBool << " " << Prints[Type(TypeId::BOOL)] << "(" << GBool << " value, int is_top); " << endl;
    *Output << GBool << " " << Prints[Type(TypeId::BOOL)] << "(" << GBool << " value, int is_top) { " << endl
            << "    print(\"%s\", (value) ? \"True\" : \"False\");" << endl
            << "    return value;" << endl
            << "} " << endl
            << endl;

    *Header << StringTypeName << "* " << Prints[FakeString] << "(" << StringTypeName << "* string, int is_top); " << endl;
    *Output << StringTypeName << "* " << Prints[FakeString] << "(" << StringTypeName << "* string, int is_top) { " << endl
            << "    char* buffer = " << allocString() << "(sizeof(char) * (string->"<< GSize << " + 1)); " << endl
            << "    int i; " << endl
            << endl
            << "    for (i = 0; i < string->" << GSize << "; i++) { " << endl
            << "        buffer[i] = (char)" << GGenerated << GAt << StringTypeName << "(string, i)->" << GValue << "; " << endl
            << "    } " << endl
            << endl
            << "    buffer[i] = '\\0'; " << endl
            << endl
            << "    if (is_top)" << endl
            << "        print(\"%s\", buffer); " << endl
            << "    else" << endl
            << "        print(\"\\\"%s\\\"\", buffer); " << endl
            << "    return string;" << endl
            << "} " << endl
            << endl;
}

string CCodeGenOld::getList(Type &Ty) {
    auto Got = Lists.find(Ty);

    if (Got == Lists.end()) {
        return generateList(Ty);
    } else {
        return Got->second;
    }
}

string CCodeGenOld::getTuple(Type &Ty) {
    auto Got = Tuples.find(Ty);

    if (Got == Tuples.end()) {
        return generateTuple(Ty);
    } else {
        return Got->second;
    }
}

string CCodeGenOld::getEnvironment(Type &Ty) {
    auto Got = Closures.find(Ty);

    if (Got == Closures.end()) {
        return generateEnvironment(Ty);
    } else {
        return Got->second;
    }

}

void CCodeGenOld::outputBuffer() {
    *Output << Buffer.str(), Buffer.str(string());
}

void CCodeGenOld::outputBeforeExpr() {
    while (BeforeExpr.size()) {
        *Output<< BeforeExpr.back();
        BeforeExpr.pop_back();
    }
}

std::string CCodeGenOld::allocString() {
    if (GC) {
        return "GC_Malloc";
    } else {
        return "malloc";
    }
}







