#include <algorithm>
#include "Test.h"

void Test::setUp() {
    // First test setup

#ifdef CCPP
    backend = compiler::Backend::CPP;
#elif CGNUASM
    backend = compiler::Backend::GNUASM;
#elif CHASKELL
    backend = compiler::Backend::HASKELL;
#elif CLLVM
    backend = compiler::Backend::LLVM;
#else
    backend = compiler::Backend::PPRINTER;
#endif
}

void Test::tearDown() {
    // Final test cleanup
}

bool Test::compileChecker(shared_ptr<std::stringstream> source) {
    int compStatus;
    std::stringstream out;
    std::stringstream hout;

    std::ofstream in("test.sppl");
    in << source->str();
    in.close();

    try {
        compiler::Compiler compiler;

        std::vector<string> in;
        in.push_back("test.sppl");
        compiler.out = &out;
        compiler.hout = &hout;

        compiler.set_inputs(in);
        // compiler.parse_file(/*file*/);

        compiler.set_backend(backend);
        compStatus = compiler.compile();
    }
    catch (...) {
        CPPUNIT_ASSERT_MESSAGE("Exception Thrown In Compiler", false);
        return false;
    }

    if (compStatus != 0) {
        return false;
    } else {

#ifdef CCPP
        // Create new files for test
        std::ofstream outC("out.c");
        std::ofstream outH("test.h");

        // And add generated code to output
        outC << out.str();
        outH << hout.str();

        outC.close();
        outH.close();
#elif CLLVM
        std::ofstream outIr("out.ir");
        outIr << out.str();
        outIr.close();
#endif
        // Return true because success
        return true;
    }
}

bool Test::executeChecker(std::string args, std::string expectedOutput) {
    return true; // TODO: DON'T DO THIS!!!

#ifdef CCPP
    return executeCPP(args, expectedOutput);
#elif CLLVM
    return executeLLVM(args, expectedOutput);
#endif
    return true;
}

bool Test::checkIfFileExists(string file) {
    ifstream f(file);
    if (f.bad()) {
        f.close();
        return false;
    } else {
        f.close();
        return true;
    }
}

std::string Test::buildCase(std::string pattern, std::string body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "| " << pattern << " = " << body << endl;
    return source->str();
}

shared_ptr<std::stringstream> Test::buildSimple(std::string signature,
                                                std::string pattern,
                                                std::string body,
                                                std::string args) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();

    int pos = signature.find_last_of("->");
    string ret = "";

    if (pos != -1) {
        ret = signature.substr(pos+1, signature.length() - 1);
    } else {
        ret = signature;
    }

    *source
    << "def main : [String]->" << ret << endl
    << "| s = func(" << args << ")" << endl
    << endl
    << "def func : " << signature << endl
    << "| " << pattern << " = " << body << endl;

    int count = 0;
    for (int pos = 0; pos < signature.length(); ++pos) {
        if (signature[pos] == '-') {
            pos++;
            if (signature[pos] == '>') {
                count++;
            }
        }
    }

    if (count > 0) {
        *source << "| ";
        for (int i = 0; i < count; ++i) {
            *source << "arg" << i << " ";
        }

        *source << " = " << body << endl;
    }

    return source;
}

shared_ptr<std::stringstream> Test::buildFunc(std::string retType,
                                              std::string func1Sig, std::string func1Pat, std::string func1Body, std::string func1Arg) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source
    << "def main : [String]->" << retType << endl
    << "| n = func1(" << func1Arg << ")" << endl
    << endl
    << "def func1 : " << func1Sig << endl
    << buildCase(func1Pat, func1Body);
    return source;
}

shared_ptr<std::stringstream> Test::buildFunc(std::string retType,
                                              std::string func1Sig, std::string func1Pat, std::string func1Body, std::string func1Arg,
                                              std::string func2Sig, std::string func2Pat, std::string func2Body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source
    << "def main : [String]->" << retType << endl
    << "| n = func1(" << func1Arg << ")" << endl
    << endl
    << "def func1 : " << func1Sig << endl
    << buildCase(func1Pat, func1Body)
    << endl
    << "def func2 : " << func2Sig << endl
    << buildCase(func2Pat, func2Body);
    return source;
}

shared_ptr<std::stringstream> Test::buildFunc(std::string retType,
                                              std::string func1Sig, std::string func1Pat, std::string func1Body, std::string func1Arg,
                                              std::string func2Sig, std::string func2Pat, std::string func2Body,
                                              std::string func3Sig, std::string func3Pat, std::string func3Body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source
    << "def main : [String]->" << retType << endl
    << "| n = func1(" << func1Arg << ")" << endl
    << endl
    << "def func1 : " << func1Sig << endl
    << buildCase(func1Pat, func1Body)
    << endl
    << "def func2 : " << func2Sig << endl
    << buildCase(func2Pat, func2Body)
    << endl
    << "def func3 : " << func3Sig << endl
    << buildCase(func3Pat, func3Body);
    return source;
}

shared_ptr<std::stringstream> Test::buildMultiCase(string signature,
                                                   std::string ret,
                                                   std::vector<string> pattern,
                                                   std::string cas,
                                                   std::string compArg) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();

    *source
    << "def main : [String]->" << ret << endl
    << "| n = func(" << compArg << ")" << endl
                                          << endl

    << "def func : " << signature << "->" << ret << endl;

    for(auto p : pattern) {
        *source << buildCase(p, cas);
    }

    return source;
}

bool Test::executeCPP(std::string args, std::string expectedOutput) {
    // Check if files exist
    if(checkIfFileExists("out.c") == false) {
        CPPUNIT_ASSERT_MESSAGE("file out.c not found", false);
    }

    if(checkIfFileExists("test.h") == false) {
        CPPUNIT_ASSERT_MESSAGE("file test.h not found", false);
    }

    // Compile program
    int status = system("gcc out.c -o prog");

    if (status != 0) {
        CPPUNIT_ASSERT_MESSAGE("C Compiler Error", false);
    }

    // SOURCE: http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
    // Used to read from outputstream for the program

    string argBuilder = "./prog ";
    argBuilder += args;
    char* arg = (char*) argBuilder.c_str();

    shared_ptr<FILE> pipe(popen(arg,"r"),pclose);
    if (!pipe) {
        CPPUNIT_ASSERT_MESSAGE("could not open pipe", false);
        return false;
    }
    string res = "";
    char buffer[128];

    while(!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) {
            res += buffer;
        }
    }

    if (res.compare(expectedOutput + "\n") != 0) {
        // If fail, first clear last test output, then return
        string resTemp = "result differ from expected\nResult   => ";
        resTemp += res;
        resTemp += "\nExpected => ";
        resTemp += expectedOutput;

        CPPUNIT_ASSERT_MESSAGE(resTemp, false);

        remove("out.c");
        remove("test.h");
        remove("prog");
        return false;
    } else {
        // If successful, first clear last test output
        remove("out.c");
        remove("test.h");
        remove("prog");
        return true;
    }
}

bool Test::executeLLVM(std::string args, std::string expectedOutput) {
    if(checkIfFileExists("out.ir") == false) {
        CPPUNIT_ASSERT_MESSAGE("file out.ir not found", false);
    }


}
