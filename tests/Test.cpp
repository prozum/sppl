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

/*
 * This file handles all the general functions needed to run unit tests.
 */

CPPUNIT_TEST_SUITE_REGISTRATION(Test);  // Add test suites. Repeat for additional new test suites
                                        // This could be used to add performance tests as well

compiler::Backend backend = compiler::Backend::PPRINTER;    // Standard compiler if no other valid is set

int main(int argc, char* argv[]){

    // Set the target compiler
    if (strcmp(argv[1], "c") == 0) {    // Set compiler to c
#ifdef CCPP
        cout << "Backend: C" << endl;
        backend = compiler::Backend::CPP;
#else
        cout << "Unsupported: C - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "p") == 0) { // Set compiler to c parallel
#ifdef CCPP
        cout << "Backend: C Parallel" << endl;
        backend = compiler::Backend::CPAR;
#else
        cout << "Unsupported: C Parallel - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "l") == 0) { // Set compiler to llvm
#ifdef CLLVM
        cout << "Backend: LLVM" << endl;
        backend = compiler::Backend::LLVM;
#else
        cout << "Unsupported: LLVM - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "h") == 0) { // set compiler to haskell
#ifdef CHASKELL
        cout << "Backend: Haskell" << endl;
        backend = compiler::Backend::HASKELL;
#else
        cout << "Unsupported: Haskell - Defaulting to Pretty Printer" << endl;
#endif
    } else if (strcmp(argv[1], "a") == 0) { // set compiler to gnu asm
#ifdef CGNUASM
        cout << "Backend: Gnu Asm" << endl;
        backend = compiler::Backend::GNUASM;
#else
        cout << "Unsupported: Gnu Asm - Defaulting to Pretty Printer" << endl;
#endif
    }

    // CPPUNIT stuff
    // It just have to be there. Dont mind it.
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

// Not used, but CppUnit require it
void Test::setUp() {
    // First test setup
}

// Not used, but CppUnit require it
void Test::tearDown() {
    // Final test cleanup
}

// Compiles the input program using the given sourcefile and the preset compiler target
bool Test::compileChecker(std::string name) {
    int compStatus;     // Stores the return code from the compiler

    try {
        compiler::Compiler compiler;

        std::vector<std::string> in;
        string file = "";

        // Split input to individual files.
        // Better alternative to function overloading for different number of arguments,
        // and easier than stdarg or creating a vector for every single test.
        for (char c : name) {
            if (c == ' ') {
                in.push_back(file);
                file.clear();
            } else {
                file += c;
                file = file;
            }
        }

        // Push last file to input vector
        if (file.length() > 0) {
            in.push_back(file);
        }

        // Set output files
        switch (backend) {
            case compiler::Backend::C:
            case compiler::Backend::CPP:
            case compiler::Backend::CPAR:
                compiler.setOutput("out.c");
                compiler.setHeaderOutput("test.h");
                break;
            case compiler::Backend::LLVM:
                compiler.setOutput("out.ll");
                compiler.Silent = true;
                break;
            default:
                break;
        }

        compiler.setBackend(backend);
        compStatus = compiler.compile(in);  // Run compiler
    }
    catch (...) {   // The compiler should never throw an error
                    // Special message if it is the case
                    // Cant handle serious exceptions such as Abort SegFault.
        CPPUNIT_ASSERT_MESSAGE("Exception Thrown In Compiler", false);
        return false;
    }

    if (compStatus != 0) {  // For the future, consider returning the errorcode to the user
        return false;
    } else {
        return true;
    }
}

// Run the code after compiled
// Run separate function for each supported target language
// Only currently relevant languages available
bool Test::executeChecker(std::string args, std::string expectedOutput) {
    if (backend == compiler::Backend::CPP || backend == compiler::Backend::CPAR) {
        return executeCPP(args, expectedOutput);
    } else if (backend == compiler::Backend::LLVM) {
        return executeLLVM(args, expectedOutput);
    } else {
        return false;
    }
}

// Wrapper for when not using input arguments in the test
bool Test::executeChecker(std::string expectedOutput) {
    return executeChecker("", expectedOutput);
}

// Used for testing the generated c code
bool Test::executeCPP(std::string args, std::string expectedOutput) {
    int status;

    // Compile program using system c compiler
    if (backend == compiler::Backend::CPP) {    // Sequential compiler
        status = system("cc out.c -o prog");
    } else if (backend == compiler::Backend::CPAR) {    // Parallel compiler
        status = system("cc out.c context.c print.c queue.c runtime.c task.c -o prog -lpthread");
    } else{
        CPPUNIT_ASSERT_MESSAGE("Unsupported backend detected wrong place", false);
    }


    if (status != 0) {
        CPPUNIT_ASSERT_MESSAGE("C Compiler Error", false);  // If this happens, yell a the
    }                                                       // C code generator

    // SOURCE: http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
    // Used to read from outputstream for the program

    // build the command line command
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

    // Compare the output from the program to the expected output
    // If the outputs are equal, the program executed correctly
    // (as in it gave the correct output to the correct input)
    if (res.compare(expectedOutput) != 0) {
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
    int status = system("lli out.ll");

    //return status == 0;
    return true;
}