#include <algorithm>
#include <iostream>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Test);

compiler::Backend backend = compiler::Backend::PPRINTER;

int main(int argc, char* argv[]){

    // Set the target compiler
    if (strcmp(argv[1], "c") == 0) {
#ifdef CCPP
        cout << "Backend: C" << endl;
        backend = compiler::Backend::CPP;
#else
        cout << "Unsupported: C - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "p") == 0) {
#ifdef CCPP
        cout << "Backend: C Parallel" << endl;
        backend = compiler::Backend::CPAR;
#else
        cout << "Unsupported: C Parallel - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "l") == 0) {
#ifdef CLLVM
        cout << "Backend: LLVM" << endl;
        backend = compiler::Backend::LLVM;
#else
        cout << "Unsupported: LLVM - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "h") == 0) {
#ifdef CHASKELL
        cout << "Backend: Haskell" << endl;
        backend = compiler::Backend::HASKELL;
#else
        cout << "Unsupported: Haskell - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "a") == 0) {
#ifdef CGNUASM
        cout << "Backend: Gnu Asm" << endl;
        backend = compiler::Backend::GNUASM;
#else
        cout << "Unsupported: Gnu Asm - Defaulting to Pretty Printer" << endl;
#endif
    }

    // CPPUNIT stuff
    CPPUNIT_NS::TestResult test_result;

    CPPUNIT_NS::TestResultCollector collector;
    test_result.addListener(&collector);

    CPPUNIT_NS::BriefTestProgressListener progress;
    test_result.addListener (&progress);

    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(test_result);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collector, std::cerr);
    compileroutputter.write ();

    return collector.wasSuccessful() ? 0 : 1;
}

void Test::setUp() {
    // First test setup
}

void Test::tearDown() {
    // Final test cleanup
}

bool Test::compileChecker(std::string name) {
    int compStatus;     // Stores the return code from the compiler

    try {
        compiler::Compiler compiler;

        std::vector<std::string> in;
        string file = "";

        for (char c : name) {
            if (c == ' ') {
                in.push_back(file);
                file.clear();
            } else {
                file += c;
                file = file;
            }
        }

        if (file.length() > 0) {
            in.push_back(file);
        }

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
    if (backend == compiler::Backend::CPP || backend == compiler::Backend::CPAR) {
        return executeCPP(args, expectedOutput);
    } else if (backend == compiler::Backend::LLVM) {
        return executeLLVM(args, expectedOutput);
    } else {
        return false;
    }
}

bool Test::executeChecker(std::string expectedOutput) {
    return executeChecker("", expectedOutput);
}

bool Test::executeCPP(std::string args, std::string expectedOutput) {
    int status;

    // Compile program
    if (backend == compiler::Backend::CPP) {
        status = system("cc out.c -o prog");
    } else if (backend == compiler::Backend::CPAR) {
        status = system("cc out.c context.c print.c queue.c runtime.c task.c -o prog -lpthread");
    } else{
        CPPUNIT_ASSERT_MESSAGE("Unsupported backend detected wrong place", false);
    }


    if (status != 0) {
        CPPUNIT_ASSERT_MESSAGE("C Compiler Error", false);
    }

    // SOURCE: http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
    // Used to read from outputstream for the program

    std::string argBuilder = "./prog ";
    argBuilder += args;
    char* arg = (char*) argBuilder.c_str();

    std::shared_ptr<FILE> pipe(popen(arg,"r"),pclose);
    if (!pipe) {
        CPPUNIT_ASSERT_MESSAGE("could not open pipe", false);
        return false;
    }
    std::string res = "";
    char buffer[128];

    while(!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) {
            res += buffer;
        }
    }

    if (res.compare(expectedOutput + "\n") != 0) {
        // If fail, first clear last test output, then return
        std::string resTemp = "result differ from expected\nResult   => ";
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
    return true; // TODO: DONT DO THIS
}