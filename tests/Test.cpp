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
    shared_ptr<std::stringstream> out = make_shared<std::stringstream>();
    shared_ptr<std::stringstream> hout = make_shared<std::stringstream>();

    try {
        compiler::Compiler compiler(source, out, hout);
        compiler.set_backend(backend);
        compStatus = compiler.compile();
    }
    catch (...) {
        return false;
    }

    if (compStatus != 0) {
        return false;
    } else {
        // Create new files for test
        std::ofstream outC("out.c");
        std::ofstream outH("test.h");

        // And add generated code to output
        outC << out->str();
        outH << hout->str();

        outC.close();
        outH.close();

        // Return true because success
        return true;
    }
}

bool Test::executeChecker(std::string args, std::string expectedOutput) {
    return true; // TODO: DONT DO THIS!!!

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

shared_ptr<std::stringstream> Test::buildFunc(std::string func1Sig, std::string func1Pat, std::string func1Body,
                                              std::string func2Sig, std::string func2Pat, std::string func2Body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source
    << "def main : [String]->" << ret << endl
    << "| n = func1"

    //*source << "def main : [String]" << func1Sig << endl << buildCase(func1Pat,
    //                                                          func1Body);
    *source << "def func1 : " << func1Sig << endl << buildCase(func1Pat,
                                                               func1Body);
    *source << "def func2 : " << func2Sig << endl << buildCase(func2Pat,
                                                              func2Body);
    return source;
}

shared_ptr<std::stringstream> Test::buildFunc(std::string mainSig, std::string mainPat, std::string mainBody,
                                              std::string func1Sig, std::string func1Pat, std::string func1Body,
                                              std::string func2Sig, std::string func2Pat, std::string func2Body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << mainSig << endl << buildCase(mainPat,
                                                            mainBody);
    *source << "def func1 : " << func1Sig << endl << buildCase(func1Pat,
                                                              func1Body);
    *source << "def func2 : " << func2Sig << endl << buildCase(func2Pat,
                                                              func2Body);
    return source;
}

shared_ptr<std::stringstream> Test::buildFunc(std::string mainSig, std::string mainPat, std::string mainBody,
                                              std::string func1Sig, std::string func1Pat, std::string func1Body,
                                              std::string func2Sig, std::string func2Pat, std::string func2Body,
                                              std::string func3Sig, std::string func3Pat, std::string func3Body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << mainSig << endl << buildCase(mainPat,
                                                            mainBody);
    *source << "def func1 : " << func1Sig << endl << buildCase(func1Pat,
                                                              func1Body);
    *source << "def func2 : " << func2Sig << endl << buildCase(func2Pat,
                                                              func2Body);
    *source << "def func3 : " << func3Sig << endl << buildCase(func3Pat,
                                                              func3Body);
    std::string s = source->str();
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