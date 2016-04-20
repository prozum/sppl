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

bool Test::compileChecker(std::string name) {
    int compStatus;

    ifstream f(name);
    if (f.bad()) {
        f.close();
        CPPUNIT_ASSERT_MESSAGE("File \"" + name + "\" not found!", false);
    } else {
        f.close();
    }

    try {
        compiler::Compiler compiler;

        std::vector<string> in;
        in.push_back(name);

        compiler.setOutput("out.c");
        compiler.setHeaderOutput("test.h");

        compiler.setBackend(backend);
        compStatus = compiler.compile(in);
    }
    catch (...) {
        CPPUNIT_ASSERT_MESSAGE("Exception Thrown In Compiler", false);
        return false;
    }

    if (compStatus != 0) {
        return false;
    } else {
        return true;
    }
}

bool Test::executeChecker(std::string args, std::string expectedOutput) {

#ifdef CCPP
    return executeCPP(args, expectedOutput);
#elif CLLVM
    return executeLLVM(args, expectedOutput);
#endif
    return true;
}

bool Test::executeChecker(std::string expectedOutput) {
    return executeChecker("", expectedOutput);
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

    return false;
}