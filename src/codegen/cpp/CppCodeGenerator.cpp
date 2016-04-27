#include "CppCodeGenerator.h"

using namespace common;
using namespace std;
using namespace codegen;

CCodeGenerator::CCodeGenerator(parser::Driver &Drv)
    : CodeGenerator(Drv), Output(Drv.Out), Header(Drv.HOut),
      ListOffsets(vector<int>({0})),
      StringList(Type(TypeId::LIST, vector<Type>({Type(TypeId::STRING)}))),
      RealString(Type(TypeId::LIST, vector<Type>({Type(TypeId::CHAR)}))) {}

void CCodeGenerator::visit(Program &Node) {
    Prog = &Node;
    Function *Main = nullptr;

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

    if (!Main)
        throw "No main, help!";

    // Get the type of main, so that return type of main is generated
    getType(Main->Signature.Subtypes.back());

    // Generate real main function, that calls the users main
    string StrListName = getList(StringList);

    *Output << "int main(int argc, char** argv) { \n"
               "    "
            << StrListName << " *args = " << GGenerated << GCreate
            << StrListName << "(0); \n"
                              "    int i; \n"
                              " \n"
                              "    for(i = argc - 1; i >= 0; i--) { \n"
                              "        args = "
            << GGenerated << GAdd << StrListName << "(args, " << GGenerated
            << GCreate << GString << "(argv[i])); \n"
                                     "    } \n"
                                     " \n"
                                     "    "
            << GGenerated << GPrint << GString << "("
            << ToStrings[Main->Signature.Subtypes.back()] << "(" << GUser << GMain << "(args))); \n"
               "    return 0; \n"
               "} \n"
               " \n";

    /*
    *Output << "int main(int argc, char** argv) { \n"
               "    "
            << StrListName << " *args = " << GGenerated << GCreate
            << StrListName << "(0); \n"
                              "    int i; \n"
                              " \n"
                              "    for(i = argc - 1; i >= 0; i--) { \n"
                              "        args = "
            << GGenerated << GAdd << StrListName << "(args, " << GGenerated
            << GCreate << GString << "(argv[i])); \n"
                                     "    } \n"
                                     " \n"
                                     "    "
            << GGenerated << GPrint << GString << "("
            << ToStrings[Main->Signature.Subtypes.back()] << "(" << GGlobal
            << GUser << GMain << ".call(&" << GGlobal << GUser << GMain
            << ", args))); \n"
               "    return 0; \n"
               "} \n"
               " \n";
     */

    for (auto &Func : Node.Decls) {
        Func->accept(*this);
        *Output << endl;
        outputBuffer();
    }
}

void CCodeGenerator::visit(Function &Node) {
    stringstream Func;
    stringstream ArgName;
    string RetType = getType(Node.Signature.Subtypes.back());
    string ArgType;
    //string Signature = getEnvironment(Node.Signature);

    CurFunc = &Node;

    // Generate function name and return type
    Func << RetType << " " << GUser << Node.Id << "(";
    // << Signature << " *" << GGenerated << GSignature;

    // Generate function arguments
    for (size_t i = 0; i < Node.Signature.subtypeCount() - 1; ++i) {
        ArgType = getType(Node.Signature.Subtypes[i]);
        ArgName << GGenerated << GArg << i;
        ArgNames.push_back(ArgName.str());
        Func << ArgType << " " << ArgName.str();

        ArgName.str("");

        if (i != Node.Signature.subtypeCount() - 2)
            Func << ", ";
    }

    Func << ")";

    // Generate function decleration in header
    *Header << Func.str() << "; \n \n";

    /*
    *Header << Signature << " " << GGlobal << GUser << Node.Id << " = { "
            << GUser << Node.Id << " }; \n\n";
            */

    // Generate function in *output
    *Output << Func.str() << " { \n"
                             "Start: \n"
                             "\n";

    // Generate cases
    for (auto &Case : Node.Cases) {
        Case->accept(*this);
        *Output << endl;

        // Clear assigments specific for current case
        Assignments.clear();
    }

    // Generate error, for when program doesn't realize a case
    *Output << "    printf(\"No cases realized!\\n\"); \n"
               "    exit(1); \n"
               "} \n"
               " \n";

    // Clear ArgNames for current function
    ArgNames.clear();
}

void CCodeGenerator::visit(Case &Node) {
    stringstream Pattern;
    bool Empty = true;

    // Generate if-statement for matching
    Pattern << "    if (";

    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        // Push arg_name on get_value_builder. get_value_builder is used for
        // generate
        // Assignments in a case
        GetValueBuilder.push_back(ArgNames[i]);

        // Generate pattern
        Node.Patterns[i]->accept(*this);

        // Cleanup
        GetValueBuilder.pop_back();

        // Only add pattern, if pattern is not "1"
        if (LastPattern != "1") {

            if (!Empty)
                Pattern << " && ";

            Empty = false;
            Pattern << LastPattern;
        }
    }

    Pattern << ")";

    // Only generate if statement, if the pattern wasn't empty
    if (!Empty) {
        *Output << Pattern.str() << endl;
    }

    *Output << "    { \n";

    // Generate all nessesary assigments
    for (auto &Assign : Assignments) {
        *Output << "        " << Assign << endl;

        if (Assign == Assignments.back())
            *Output << endl;
    }

    string ExtraTap;

    if (Node.When) {
        // Generate return expression
        ExprStack.push(stringstream());

        Node.When->accept(*this);
        *Output << "        if (" << ExprStack.top().str() << ") \n"
                   "        { \n";
        ExprStack.pop();

        ExtraTap = "    ";
    }

    // Generate return expression
    ExprStack.push(stringstream());

    if (Node.TailRec) {
        auto C = (CallExpr *)Node.Expr.get();
        for (size_t i = 0; i < CurFunc->Signature.Subtypes.size() - 1; ++i) {
            ExprStack.push(stringstream());
            C->Args[i]->accept(*this);
            *Output << "        " << ExtraTap << GGenerated << GArg << i << " = "
                    << ExprStack.top().str() << "; \n";
            ExprStack.pop();
        }

        *Output << "\n"
                   "        goto Start; \n";
    } else {
        string ResType = getType(CurFunc->Signature.Subtypes.back());
        ExprStack.push(stringstream());

        Node.Expr->accept(*this);
        *Output << "        " << ExtraTap << ResType << " res = " << ExprStack.top().str()
                << "; \n";
        *Output << "        " << ExtraTap << "return res; \n";
        ExprStack.pop();
    }

    if (Node.When) {
        *Output << "        } \n";
    }

    *Output << "    } \n"
               " \n";
}

void CCodeGenerator::visit(AlgebraicDT &Node) {
    throw std::runtime_error("Not implemented");
}

void CCodeGenerator::visit(Product &Node) {
    throw std::runtime_error("Not implemented");
}

string CCodeGenerator::getType(Type &Ty) {
    // Generate the currect c type, based on sppl's types
    switch (Ty.Id) {
    case TypeId::FLOAT:
        return GFloat;
    case TypeId::CHAR:
        return GChar;
    case TypeId::INT:
        return GInt;
    case TypeId::BOOL:
        return GBool;
    // For tuples, lists, closures and strings, custom types will be generated
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
        // mistake.
        throw runtime_error("This should never happen!");
    }
}

string CCodeGenerator::generateList(Type &Ty) {
    // Save the generated list in a result stream
    stringstream Res;
    string Name = GGenerated + GList + to_string(ListCount);
    string ChildrenType = getType(Ty.Subtypes.front());

    // Increase list count, so next list doesn't have the same name
    ListCount++;

    // Generate the list struct
    *Header << " \n"
               "typedef struct "
            << Name << " { \n"
                       "    struct "
            << Name << "* " << GNext << "; \n"
                                        "    "
            << ChildrenType << " " << GValue << "; \n"
                                                "    int "
            << GEmpty << "; \n"
                         "    int "
            << GSize << "; \n"
                        "} "
            << Name << "; \n"
                       " \n";

    // Generate add function for list
    *Header << Name << "* " << GGenerated << GAdd << Name << "(" << Name
            << "* current, " << ChildrenType << " item); \n";
    Buffer << Name << "* " << GGenerated << GAdd << Name << "(" << Name
           << "* current, " << ChildrenType << " item) { \n"
                                               "    "
           << Name << "* res = malloc(sizeof(" << Name << "));\n"
                                                          "    res->"
           << GValue << " = item; \n"
                        "    res->"
           << GNext << " = current; \n"
                       "    res->"
           << GEmpty << " = 0; \n"
                        "    res->"
           << GSize << " = current->" << GSize << " + 1; \n"
                                                  "    return res; \n"
                                                  "} \n"
                                                  " \n";

    // Generate create function for list
    *Header << Name << "* " << GGenerated << GCreate << Name
            << "(int count, ...); \n";
    Buffer << Name << "* " << GGenerated << GCreate << Name
           << "(int count, ...) { \n"
              "    int i; \n"
              "    va_list args; \n"
              "    "
           << Name << "* res = malloc(sizeof(" << Name << ")); \n"
                                                          "    res->"
           << GEmpty << " = 1; \n"
                        "    res->"
           << GSize << " = 0; \n"
                       "\n"
                       "    va_start(args, count); \n"
                       "\n"
                       "    for (i = 0; i < count; i++) { \n"
                       "        res = "
           << GGenerated << GAdd << Name << "(res, va_arg(args, "
           << ChildrenType << ")); \n"
                              "    } \n"
                              "\n"
                              "    va_end(args); \n"
                              "    return res; \n"
                              "} \n"
                              " \n";

    // Generate at function for list
    *Header << Name << "* " << GGenerated << GAt << Name << "(" << Name
            << "* current, int index); \n";
    Buffer << Name << "* " << GGenerated << GAt << Name << "(" << Name
           << "* current, int index) { \n"
              "    while (index-- > 0) { \n"
              "        if (current->"
           << GEmpty << ") { \n"
                        "            return current; \n"
                        "        } \n"
                        "\n"
                        "        current = current->"
           << GNext << "; \n"
                       "    } \n"
                       "\n"
                       "    return current; \n"
                       "} \n"
                       " \n";

    // Generate valueat function for list
    *Header << ChildrenType << " " << GGenerated << GValueAt << Name << "("
            << Name << "* current, int index); \n";
    Buffer << ChildrenType << " " << GGenerated << GValueAt << Name << "("
           << Name << "* current, int index) { \n"
                      "    "
           << Name << "* res = " << GGenerated << GAt << Name
           << "(current, index); \n"
              "\n"
              "    if (res->"
           << GEmpty << ") { \n"
                        "        printf(\"Out of bound! "
           << OobCount++ << "\\n\"); \n"
                            "        exit(1); \n"
                            "    }"
                            "\n"
                            "    return res->"
           << GValue << "; \n"
                        "} \n"
                        " \n";

    // Generate compare function for list
    *Header << "int " << GGenerated << GCompare << Name << "(" << Name
            << "* list1, " << Name << "* list2); \n";
    Buffer << "int " << GGenerated << GCompare << Name << "(" << Name
           << "* list1, " << Name << "* list2) { \n"
                                     "    int i; \n"
                                     "    if (list1->"
           << GSize << " != list2->" << GSize << ") { \n"
                                                 "         return 0; \n"
                                                 "    } \n"
                                                 " \n"
                                                 "    for (i = 0; i < list1->"
           << GSize << "; i++) { \n"
                       "        if(";

    switch (Ty.Subtypes.front().Id) {
    case TypeId::LIST:
    case TypeId::TUPLE:
        Buffer << "!" << GGenerated << GCompare << ChildrenType << "("
               << GGenerated << GValueAt << Name << "(list1, i), " << GGenerated
               << GValueAt << Name << "(list2, i))";
        break;
    default:
        Buffer << GGenerated << GValueAt << Name
               << "(list1, i) != " << GGenerated << GValueAt << Name
               << "(list2, i)";
        break;
    }

    Buffer << ") \n"
              "            return 0; \n"
              "    } \n"
              " \n"
              "    return 1; \n"
              "} \n"
              " \n";

    // Generation of concat
    *Header << Name << "* " << GGenerated << GConcat << Name << "(" << Name
            << "* list1, " << Name << "* list2); \n";
    Buffer << Name << "* " << GGenerated << GConcat << Name << "(" << Name
           << "* list1, " << Name << "* list2) { \n"
                                     "    int i; \n"
                                     "    "
           << Name << "** elements = malloc(sizeof(" << Name << "*) * list1->"
           << GSize << "); \n"
                       " \n"
                       "    for (i = 0; !list1->"
           << GEmpty << "; ++i) { \n"
                        "        elements[i] = list1; \n"
                        "        list1 = list1->"
           << GNext << "; \n"
                       "    } \n"
                       " \n"
                       "    for (--i; i >= 0; --i) { \n"
                       "        list2 = "
           << GGenerated << GAdd << Name << "(list2, elements[i]->" << GValue
           << "); \n"
              "    } \n"
              " \n"
              "    free(elements); \n"
              "    return list2; \n"
              "} \n"
              "\n";

    // generation of tostring
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
            << "* value); \n";
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
           << "* value) { \n"
              "    "
           << StringTypeName << "* comma = " << GGenerated << GCreate << GString
           << "(\", \"); \n"
              "    "
           << StringTypeName << "* res = " << GGenerated << GCreate << GString
           << "(\"]\"); \n"
              "    int i; \n"
              "\n"
              "    for (i = value->"
           << GSize << " - 1; i >= 0; i--) { \n"
                       "        res = "
           << GGenerated << GConcat << StringTypeName << "("
           << ToStrings[Ty.Subtypes.front()] << "(" << GGenerated << GValueAt
           << Name << "(value, i)), res); \n"
                      " \n"
                      "        if (i != 0) \n"
                      "            res = "
           << GGenerated << GConcat << StringTypeName << "(comma, res); \n"
                                                         "    } \n"
                                                         " \n"
                                                         "    res = "
           << GGenerated << GAdd << StringTypeName << "(res, '['); \n"
                                                      "    return res; \n"
                                                      "} \n"
                                                      " \n";

    Lists[Ty] = Name;

    // Return name of list generated
    return Name;
}

string CCodeGenerator::generateEnvironment(Type &Ty) {
    string Name = GGenerated + GSignature + to_string(++SigCount);
    stringstream Res;

    Res << "typedef " << getType(Ty.Subtypes.back()) << "(*" << Name << ")(";

    for (size_t i = 0; i < Ty.subtypeCount() - 1; ++i) {
        Res << getType(Ty.Subtypes[i]);

        if (i != Ty.subtypeCount() - 2)
            Res << ", ";
    }
    
    Res << "); \n";

    *Header << Res.str();

    Closures[Ty] = Name;
    return Name;
/*
    // Result is needed, so we don't generate something inside the signature,
    // while generating other types
    stringstream Res;
    string Name = GGenerated + GSignature + to_string(SigCount);

    // Increase signature count, so next signature doesn't have the same name
    SigCount++;

    Res << "typedef struct " << Name << " {\n"
        << "    " << getType(Ty.Subtypes.back()) << " (* call)(struct " << Name
        << "*";

    for (size_t i = 0; i < Ty.Subtypes.size() - 1; ++i) {
        Res << ", ";

        // Get the actual type of the argument
        Res << getType(Ty.Subtypes[i]);
    }

    Res << "); \n"
           "} "
        << Name << ";\n\n";

    *Header << "\n" << Res.str();

    // Generation of signature to string
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
            << " value); \n";
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
           << " value) { \n"
              "    return "
           << GGenerated << GCreate << GString << "(\"" << Ty.str() << "\"); \n"
                                                                       "} \n"
                                                                       " \n";

    // Save signature in signature hash map
    Closures[Ty] = Name;

    // Return name of signature generated
    return Name;
 */
}

string CCodeGenerator::generateTuple(Type &Ty) {
    // Result is needed, so we don't start generating something while generating
    // the tuple
    stringstream Res;
    string Name = GGenerated + GTuple + to_string(TupleCount);

    // Increase tuple count, so next tuple doesn't have the same name
    TupleCount++;

    // Generate the tuple struct
    Res << " \n"
           "typedef struct "
        << Name << " {" << endl;
    // Generate an item for each type in the tuple
    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        // Get the actual type of the item
        Res << "    " << getType(Ty.Subtypes[i]) << " " << GGenerated << GItem
            << i << ";" << endl; // give this item a unique name
    }
    Res << "} " << Name << "; \n";

    *Header << Res.str();
    Res.str("");

    // Generate a create function for the tuple
    // Give contructor a unique name
    Res << Name << " " << GGenerated << GCreate << Name << "(";

    // Generate an argument for each item in the struct
    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        // Get the actual type of the argument
        Res << getType(Ty.Subtypes[i]) << " " << GGenerated << GItem << i;

        if (i <
            Ty.Subtypes.size() - 1) // Don't print ", " after the last argument
            Res << ", ";
    }

    Res << ")";

    *Header << Res.str() << "; \n";
    Buffer << Res.str() << " { \n";

    // Generate a result variable
    Buffer << "    " << Name << " "
           << "res; \n";

    // For each item in res, assign values
    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        Buffer << "    res." << GGenerated << GItem << i << " = " << GGenerated
               << GItem << i << "; \n";
    }
    Buffer << " \n"
              "    return res; \n"
              "} \n"
              " \n";

    // Generate a compare function for this tuple
    *Header << "int " << GGenerated << GCompare << Name << "(" << Name
            << " tuple1, " << Name << " tuple2); \n";
    Buffer << "int " << GGenerated << GCompare << Name << "(" << Name
           << " tuple1, " << Name << " tuple2) { \n";

    for (size_t i = 0; i < Ty.Subtypes.size(); ++i) {
        Buffer << "    if (";

        switch (Ty.Subtypes[i].Id) {
        case TypeId::STRING:
        case TypeId::LIST:
            Buffer << "!" << GGenerated << GCompare << Lists[Ty.Subtypes[i]]
                   << "(tuple1." << GGenerated << GItem << i << ", tuple2."
                   << GGenerated << GItem << i << ")";
            break;
        case TypeId::TUPLE:
            Buffer << "!" << GGenerated << GCompare << Tuples[Ty.Subtypes[i]]
                   << "(tuple1." << GGenerated << GItem << i << ", tuple2."
                   << GGenerated << GItem << i << ")";
            break;
        default:
            Buffer << "(tuple1." << GGenerated << GItem << i << " != tuple2."
                   << GGenerated << GItem << i << ")";
            break;
        }

        Buffer << ") \n"
                  "        return 0; \n";
    }

    Buffer << " \n"
              "    return 1; \n"
              "} \n"
              "\n";

    // Generate a to_string for the tuple
    ToStrings[Ty] = GGenerated + GToString + Name;

    *Header << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
            << " value); \n";
    Buffer << StringTypeName << "* " << ToStrings[Ty] << "(" << Name
           << " value) { \n"
              "    "
           << StringTypeName << "* comma = " << GGenerated << GCreate << GString
           << "(\", \"); \n"
              "    "
           << StringTypeName << "* res = " << GGenerated << GCreate << GString
           << "(\")\"); \n"
              "\n";

    for (size_t i = Ty.Subtypes.size() - 1; i != 0; --i) {
        Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "("
               << ToStrings[Ty.Subtypes[i]] << "(value." << GGenerated << GItem
               << i << ")"
               << ", res); \n";
        Buffer << "    res = " << GGenerated << GConcat << StringTypeName
               << "(comma, res); \n";
    }

    Buffer << "    res = " << GGenerated << GConcat << StringTypeName << "("
           << ToStrings[Ty.Subtypes[0]] << "(value." << GGenerated << GItem
           << "0)"
           << ", res); \n"
              "    res = "
           << GGenerated << GAdd << StringTypeName << "(res, '('); \n"
                                                      " \n"
                                                      "    return res; \n"
                                                      "} \n"
                                                      "\n";

    // Save tuple in tuple hash map
    Tuples[Ty] = Name;

    // Return name of tuple generated
    return Name;
}

void CCodeGenerator::generateStd() {
    *Output << "#include \"test.h\" \n \n";

    *Header << "#include <stdarg.h> \n"
               "#include <stdio.h> \n"
               "#include <stdlib.h> \n"
               "#include <string.h> \n"
               "#include <stdint.h> \n"
               "#include <inttypes.h> \n";

    ToStrings[Type(TypeId::INT)] = GGenerated + GToString + "int";
    ToStrings[Type(TypeId::BOOL)] = GGenerated + GToString + "bool";
    ToStrings[Type(TypeId::FLOAT)] = GGenerated + GToString + "float";
    ToStrings[Type(TypeId::CHAR)] = GGenerated + GToString + "char";
    ToStrings[Type(TypeId::STRING)] = GGenerated + GToString + "string";

    StringTypeName = GGenerated + GList + to_string(ListCount);
    Lists[Type(TypeId::STRING)] = StringTypeName;

    generateList(RealString);
    generateList(StringList);

    outputBuffer();

    // Generation of string constructer starts here
    *Header << StringTypeName << "* " << GGenerated << GCreate << GString
            << "(char* values);\n";
    *Output << StringTypeName << "* " << GGenerated << GCreate << GString
            << "(char* values) { \n"
               "    int i, str_length = strlen(values); \n"
               "    "
            << StringTypeName << "* res = " << GGenerated << GCreate
            << StringTypeName
            << "(0); \n"
               " \n"
               "    for (i = str_length - 1; i >= 0; i--) { \n"
               "        res = "
            << GGenerated << GAdd << StringTypeName << "(res, values[i]); \n"
                                                       "    } \n"
                                                       " \n"
                                                       "    return res; \n"
                                                       "} \n"
                                                       " \n";

    // Generation of string compare
    *Header << "int " << GGenerated << GCompare << GString << "("
            << StringTypeName << "* string, char* values, int offset); \n";
    *Output << "int " << GGenerated << GCompare << GString << "("
            << StringTypeName << "* string, char* values, int offset) { \n"
                                 "    int i, size = strlen(values); \n"
                                 " \n"
                                 "    if (size == string->"
            << GSize << ") { \n"
                        "        for (i = 0; i < size; i++) { \n"
                        "            if ("
            << GGenerated << GValueAt << StringTypeName
            << "(string, i) != values[i]) \n"
               "                return 0; \n"
               "        } \n"
               "    } else { \n"
               "        return 0; \n"
               "    } \n"
               " \n"
               "    return 1; \n"
               "} \n"
               " \n";

    // Generation of print string
    *Header << StringTypeName << "* " << GGenerated << GPrint << GString << "("
            << StringTypeName << "* string); \n";
    *Output << StringTypeName << "* " << GGenerated << GPrint << GString << "("
            << StringTypeName
            << "* string) { \n"
               "    char* buffer = malloc(sizeof(char) * (string->"
            << GSize << " + 1)); \n"
                        "    int i; \n"
                        " \n"
                        "    for (i = 0; i < string->"
            << GSize << "; i++) { \n"
                        "        buffer[i] = (char)"
            << GGenerated << GValueAt << StringTypeName
            << "(string, i); \n"
               "    } \n"
               " \n"
               "    buffer[i] = '\\0'; \n"
               "    printf(\"%s\\n\", buffer); \n"
               "    free(buffer); \n"
               "    return string; \n"
               "} \n"
               " \n";

    // Generation of default to_string methods
    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::INT)] << "("
            << GInt << " value); \n";
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::INT)] << "("
            << GInt << " value) { \n"
                       "    char buffer[100]; \n"
                       "    sprintf(buffer, \"%\" PRId64 \"\", value); \n"
                       "    return "
            << GGenerated << GCreate << GString << "(buffer); \n"
                                                   "} \n"
                                                   " \n";

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::BOOL)] << "("
            << GBool << " value); \n";
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::BOOL)] << "("
            << GBool << " value) { \n"
                        "    if (value) \n"
                        "        return "
            << GGenerated << GCreate << GString << "(\"True\"); \n"
                                                   "    else \n"
                                                   "        return "
            << GGenerated << GCreate << GString << "(\"False\"); \n"
                                                   "} \n"
                                                   " \n";

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::FLOAT)] << "("
            << GFloat << " value); \n";
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::FLOAT)] << "("
            << GFloat << " value) { \n"
                         "    char buffer[100]; \n"
                         "    sprintf(buffer, \"%lf\", value); \n"
                         "    return "
            << GGenerated << GCreate << GString << "(buffer); \n"
                                                   "} \n"
                                                   " \n";

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::CHAR)] << "("
            << GChar << " value); \n";
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::CHAR)] << "("
            << GChar << " value) { \n"
                        "    "
            << StringTypeName << "* res = " << GGenerated << GCreate << GString
            << "(\"\\'\"); \n"
               "    res = "
            << GGenerated << GAdd << StringTypeName << "(res, value); \n"
                                                       "    res = "
            << GGenerated << GAdd << StringTypeName << "(res, '\\''); \n"
                                                       "    return res; \n"
                                                       "} \n"
                                                       " \n";

    *Header << StringTypeName << "* " << ToStrings[Type(TypeId::STRING)] << "("
            << StringTypeName << "* value); \n";
    *Output << StringTypeName << "* " << ToStrings[Type(TypeId::STRING)] << "("
            << StringTypeName << "* value) { \n"
                                 "    "
            << StringTypeName << "* res = " << GGenerated << GCreate << GString
            << "(\"\\\"\"); \n"
               "    res = "
            << GGenerated << GConcat << StringTypeName << "(value, res); \n"
                                                          "    res = "
            << GGenerated << GAdd << StringTypeName << "(res, '\"'); \n"
                                                       "} \n"
                                                       " \n";
}

string CCodeGenerator::getList(Type &Ty) {
    auto Got = Lists.find(Ty);

    if (Got == Lists.end()) {
        return generateList(Ty);
    } else {
        return Got->second;
    }
}

string CCodeGenerator::getTuple(Type &Ty) {
    auto Got = Tuples.find(Ty);

    if (Got == Tuples.end()) {
        return generateTuple(Ty);
    } else {
        return Got->second;
    }
}

string CCodeGenerator::getEnvironment(Type &Ty) {

    auto Got = Closures.find(Ty);

    if (Got == Closures.end()) {
        return generateEnvironment(Ty);
    } else {
        return Got->second;
    }

}

void CCodeGenerator::outputBuffer() {
    *Output << Buffer.str(), Buffer.str(string());
}