#include "Test.h"

void Test::setUp() {
    // First test setup
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

bool Test::executeChecker(bool compStatus, std::string args, std::string expectedOutput) {
    if (compStatus == false) {  // if the code didn't compile, we have no code to test.
        CPPUNIT_ASSERT_MESSAGE("compStatus is false, something very bad happened", false);
        return false;
    }

    // Check if out.c exist
    ifstream f("out.c");
    if (f.bad()) {
        f.close();
        CPPUNIT_ASSERT_MESSAGE("file out.c not found", false);
        return false;
    } else {
        f.close();
    }

    // Check if test.h exist
    ifstream h("test.h");
    if (h.bad()) {
        h.close();
        CPPUNIT_ASSERT_MESSAGE("file test.h not found", false);
        return false;
    } else {
        h.close();
    }

    system("gcc out.c -o prog");

    // SOURCE: http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c

    string argTemp = "./prog ";
    argTemp += args;
    char* arg = (char*)argTemp.c_str();

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
        string resTemp = "result differ from expected - ";
        resTemp += res;
        resTemp += " - ";
        resTemp += expectedOutput;
        CPPUNIT_ASSERT_MESSAGE(resTemp, false);
        remove("out.c");
        remove("test.h");
        return false;
    }

    // If successful, first clear last test output
    remove("out.c");
    remove("test.h");
    return true;   // TODO: fix
}

shared_ptr<std::stringstream> Test::buildSimple(std::string signature,
                                    std::string body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << signature << endl << "| = " << body << endl;
    return source;
}

shared_ptr<std::stringstream> Test::buildSimple(std::string signature,
                                    std::string pattern,
                                    std::string body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << signature << endl << "| " << pattern << " = " << body << endl;
    return source;
}

shared_ptr<std::stringstream> Test::buildSimple(std::string pattern,
                                    std::string left,
                                    std::string op,
                                    std::string right) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << pattern << " | = " << left << " " << op << " " << right;
    return source;
}

std::string Test::buildCase(std::string pattern, std::string body) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "| " << pattern << " = " << body << endl;
    return source->str();
}

shared_ptr<std::stringstream> Test::buildMultiCase(string signature,
                                       std::string ret,
                                       std::vector<string> pattern,
                                       std::string cas) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << signature << "->" << ret << endl;

    for(auto p : pattern) {
        *source << buildCase(p,
                  cas);
    }

    return source;
}
shared_ptr<std::stringstream> Test::buildMultiCase(std::vector<string> signature,
                                       std::string ret,
                                       std::string pattern,
                                       std::string cas) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();

    *source << "def main : ";
    for (auto s : signature) {
        *source << s << "->";
    }
    *source << ret << endl;

    *source << buildCase(pattern,
              cas);

    return source;
}

shared_ptr<std::stringstream> Test::buildMultiFunc(std::string mainSig,
                                       std::string mainPat,
                                       std::string mainBody,
                                       std::string funcSig,
                                       std::string funcPat,
                                       std::string funcBody) {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : " << mainSig << endl << buildCase(mainPat,
                                                            mainBody);
    *source << "def func : " << funcSig << endl << buildCase(funcPat,
                                                            funcBody);
    return source;
}

shared_ptr<std::stringstream> Test::buildMultiFunc(std::string mainSig, std::string mainPat, std::string mainBody,
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

shared_ptr<std::stringstream> Test::buildMultiFunc(std::string mainSig, std::string mainPat, std::string mainBody,
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

/*
 * ADDITION
 */

// Addition - Integer

void Test::addIntPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "+", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compile",compStatus == true);
    bool execStatus = executeChecker(compStatus, "", "4");
    CPPUNIT_ASSERT_MESSAGE("Execute", execStatus);
}

void Test::addIntZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "0", "+", "0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addIntPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "+", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addIntNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "+", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addIntNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "+", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Addition - Float

void Test::addFloatPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "+", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addFloatZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "0.0", "+", "0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addFloatPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "+", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addFloatNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "+", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::addFloatNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "+", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Addition - Boolean

void Test::addBoolTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "+", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addBoolTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "+", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addBoolFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "+", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addBoolFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "+", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Addition - Character

void Test::addCharStrStr() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "+", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addCharStrChar() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "+", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addCharCharStr() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "+", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addCharCharChar() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "+", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Addition - Mix

void Test::addMixFloatInt() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "+", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::addMixIntFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "+", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * SUBTRACTION
 */

// Subtraction - Integer

void Test::subIntPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "-", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subIntZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "0", "-", "0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subIntPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "-", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subIntNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "-", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subIntNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "-", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Subtraction - Float

void Test::subFloatPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "-", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subFloatZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "0.0", "-", "0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subFloatPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "-", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subFloatNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "-", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::subFloatNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "-", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Subtraction - Boolean

void Test::subBoolTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "-", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subBoolTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "-", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subBoolFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "-", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subBoolFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "-", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Subtraction - Character

void Test::subCharStrStr() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "-", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subCharStrChar() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "-", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subCharCharStr() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "-", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subCharCharChar() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "-", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Subtraction - Mix

void Test::subMixFloatInt() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "-", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::subMixIntFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "-", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * Multiplication
 */

// Multiplication - Integer

void Test::mulIntPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "*", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulIntZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "0", "*", "0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulIntPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "*", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulIntNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "*", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulIntNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "*", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Multiplication - Float

void Test::mulFloatPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "*", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulFloatZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "0.0", "*", "0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulFloatPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "*", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulFloatNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "*", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::mulFloatNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "*", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Multiplication - Boolean

void Test::mulBoolTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "*", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::mulBoolTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "*", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::mulBoolFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "*", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::mulBoolFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "*", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Multiplication - Character

void Test::mulCharStrStr() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "*", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::mulCharStrChar() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "*", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

void Test::mulCharCharStr() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "*", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

void Test::mulCharCharChar() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "*", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

// Multiplication - Mix

void Test::mulMixFloatInt() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "*", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::mulMixIntFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "*", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * Division
 */

// Division - Integer

void Test::divIntPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "/", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divIntZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "0", "/", "0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divIntPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "/", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divIntNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "/", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divIntNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "/", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Division - Float

void Test::divFloatPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "/", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divFloatZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "0.0", "/", "0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divFloatPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "/", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divFloatNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "/", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::divFloatNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "/", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Division - Boolean

void Test::divBoolTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "/", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::divBoolTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "/", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::divBoolFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "/", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::divBoolFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "/", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Division - Character

void Test::divCharStrStr() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "/", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::divCharStrChar() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "/", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

void Test::divCharCharStr() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "/", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

void Test::divCharCharChar() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "/", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

// Division - Mix

void Test::divMixFloatInt() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "/", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::divMixIntFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "/", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * Modulo
 */

// Modulo - Integer

void Test::modIntPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "%", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modIntZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "0", "%", "0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modIntPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "%", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modIntNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "%", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modIntNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "-2", "%", "-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Modulo - Float

void Test::modFloatPosPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "%", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modFloatZeroZero() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "0.0", "%", "0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modFloatPosNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "%", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modFloatNegPos() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "%", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::modFloatNegNeg() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "-2.0", "%", "-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Modulo - Boolean

void Test::modBoolTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "%", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::modBoolTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "%", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::modBoolFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "%", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::modBoolFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "%", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Modulo - Character

void Test::modCharStrStr() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "%", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::modCharStrChar() {
    shared_ptr<std::stringstream> source = buildSimple("String", "\"string\"", "%", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

void Test::modCharCharStr() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "%", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

void Test::modCharCharChar() {
    shared_ptr<std::stringstream> source = buildSimple("Char", "'c'", "%", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);

}

// Modulo - Mix

void Test::modMixFloatInt() {
    shared_ptr<std::stringstream> source = buildSimple("Float", "2.0", "%", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::modMixIntFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "%", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * Binary/Bool Operators
 */

// Binary/Bool - Equal

void Test::binEqualTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "==", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binEqualTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "==", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binEqualFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "==", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binEqualFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "==", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Binary/Bool - NotEqual

void Test::binNotEqualTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "!=", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binNotEqualTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "!=", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binNotEqualFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "!=", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binNotEqualFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "!=", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Binary/Bool - And

void Test::binAndTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "&&", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binAndTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "&&", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binAndFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "&&", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binAndFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "&&", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Binary/Bool - Or

void Test::binOrTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "||", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binOrTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "||", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binOrFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "||", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binOrFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "||", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Binary/Bool - GrEq

void Test::binGrEqTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", ">=", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binGrEqTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", ">=", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binGrEqFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", ">=", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binGrEqFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", ">=", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Binary/Bool - LeEq

void Test::binLeEqTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "<=", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLeEqTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "<=", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLeEqFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "<=", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLeEqFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "<=", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Binary/Bool - Great

void Test::binGreatTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", ">", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binGreatTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", ">", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binGreatFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", ">", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binGreatFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", ">", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Binary/Bool - Less

void Test::binLessTrueTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "<", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLessTrueFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "True", "<", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLessFalseTrue() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "<", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLessFalseFalse() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "False", "<", "False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * Binary/Int
 */

// Binary/Int

void Test::binEqualInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "==", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binNotEqualInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "!=", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binAndInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "&&", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binOrInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "||", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLeEqInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "<=", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binGrEqInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", ">=", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binLessInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "<", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binGreatInt() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", ">", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Binary/Float

void Test::binEqualFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "==", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binNotEqualFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "!=", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binAndFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "&&", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binOrFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "||", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::binLeEqFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "<=", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binGrEqFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", ">=", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binLessFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", "<", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::binGreatFloat() {
    shared_ptr<std::stringstream> source = buildSimple("Bool", "2", ">", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

/*
 * Case & Signature test
 */

// SIgnature length

void Test::sigLengthZeroInt() {
    std::vector<string> signature;

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthOneInt() {
    std::vector<string> signature;
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthTwoInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthThreeInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthFourInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthFiveInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d e", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthSixInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d e f", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthSevenInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d e f g", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthEightInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d e f g h", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthNineInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d e f g h i", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::sigLengthTenInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c d e f g h i j", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

// Pattern Match

void Test::patLengthSameInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::patLengthLessInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::patLengthMoreInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");

    shared_ptr<std::stringstream> source = buildMultiCase(signature, "Int", "a b c", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Case Count

void Test::casOneCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    string prog = source->str();
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casTwoCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casThreeCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casFourCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casFiveCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casSixCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casSevenCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casEightCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    pattern.push_back("8");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casNineCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    pattern.push_back("8");
    pattern.push_back("9");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::casTenCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    pattern.push_back("8");
    pattern.push_back("9");
    pattern.push_back("n");

    shared_ptr<std::stringstream> source = buildMultiCase("Int","Int",pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

/*
 * List
 */

void Test::listCasEmpty() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]->Int", "[]", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listAppendEmpty() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]", "1:[]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]", "[2]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[Float]", "[2.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listBool() {
    shared_ptr<std::stringstream> source = buildSimple("[Bool]", "[True]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listChar() {
    shared_ptr<std::stringstream> source = buildSimple("[Char]", "['c']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listString() {
    shared_ptr<std::stringstream> source = buildSimple("[String]", "[\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listListInt() {
    shared_ptr<std::stringstream> source = buildSimple("[[Int]]", "[[2]]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listListFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[[Float]]", "[[2.0]]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listListBool() {
    shared_ptr<std::stringstream> source = buildSimple("[[Bool]]", "[[True]]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listListChar() {
    shared_ptr<std::stringstream> source = buildSimple("[[Char]]", "[['c']]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listListString() {
    shared_ptr<std::stringstream> source = buildSimple("[[String]]", "[[\"string\"]]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listListListInt() {
    shared_ptr<std::stringstream> source = buildSimple("[[[Int]]]", "[[[2]]]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listAppendInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]", "1:[2,3]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listAppendFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[Float]", "1.0:[2.0,3.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listAppendBool() {
    shared_ptr<std::stringstream> source = buildSimple("[Bool]","True:[True,False]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listAppendChar() {
    shared_ptr<std::stringstream> source = buildSimple("[Char]", "'c':['h','a','r']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listAppendString() {
    shared_ptr<std::stringstream> source = buildSimple("[String]", "\"String\":[\"Int\",\"Float\",\"Bool\",\"Char\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listLongInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]","[0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listFloatInInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]","[2.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listBoolInInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]","[True]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listCharInInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]","['c']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listStringInInt() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]","[\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listIntInFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[Float]","[2]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listBoolInFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[Float]","[True]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listCharInFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[Float]","['c']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listStringInFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[Float]","[\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listIntInBool() {
    shared_ptr<std::stringstream> source = buildSimple("[Bool]","[2]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listFloatInBool() {
    shared_ptr<std::stringstream> source = buildSimple("[Bool]","[2.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listCharInBool() {
    shared_ptr<std::stringstream> source = buildSimple("[Bool]","['c']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listStringInBool() {
    shared_ptr<std::stringstream> source = buildSimple("[Bool]","[\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listIntInChar() {
    shared_ptr<std::stringstream> source = buildSimple("[Char]","[2]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listFloatInChar() {
    shared_ptr<std::stringstream> source = buildSimple("[Char]","[2.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listBoolInChar() {
    shared_ptr<std::stringstream> source = buildSimple("[Char]","[True]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listStringInChar() {
    shared_ptr<std::stringstream> source = buildSimple("[Char]","[\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listIntInString() {
    shared_ptr<std::stringstream> source = buildSimple("[String]","[2]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listFloatInString() {
    shared_ptr<std::stringstream> source = buildSimple("[String]","[2.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listBoolInString() {
    shared_ptr<std::stringstream> source = buildSimple("[String]","[True]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listCharInString() {
    shared_ptr<std::stringstream> source = buildSimple("[String]","['c']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listMixTypes() {
    shared_ptr<std::stringstream> source = buildSimple("[Int]", "[2,2.0,False,'c',\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::listNestedDifLengthInt() {
    shared_ptr<std::stringstream> source = buildSimple("[[Int]]","[[1,2,3],[1,2,3,4,5,6,7,8,9,0]]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listTupleInt() {
    shared_ptr<std::stringstream> source = buildSimple("[(Int,Int)]","[(1,2),(3,4),(5,6)]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listTupleFloat() {
    shared_ptr<std::stringstream> source = buildSimple("[(Float,Float)]","[(1.0,2.0),(3.0,4.0),(5.0,6.0)]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listTupleBool() {
    shared_ptr<std::stringstream> source = buildSimple("[(Bool,Bool)]","[(True,True),(True,False),(False,True),(False,False)]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listTupleChar() {
    shared_ptr<std::stringstream> source = buildSimple("[(Char,Char)]","[('c','h'),('a','r')]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::listTupleString() {
    shared_ptr<std::stringstream> source = buildSimple("[(String,String)]","[(\"string\",\"string\")]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}


/*
 * Tuple
 */

void Test::tupleEmpty() {
    shared_ptr<std::stringstream> source = buildSimple("()","()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleEmptyInt() {
    shared_ptr<std::stringstream> source = buildSimple("(Int)","()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleEmptyFloat() {
    shared_ptr<std::stringstream> source = buildSimple("(Float)","()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleEmptyBool() {
    shared_ptr<std::stringstream> source = buildSimple("(Bool)","()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleEmptyChar() {
    shared_ptr<std::stringstream> source = buildSimple("(Char)","()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleEmptyString() {
    shared_ptr<std::stringstream> source = buildSimple("(String)","()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleOneInt() {
    shared_ptr<std::stringstream> source = buildSimple("(Int)","(2)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleOneFloat() {
    shared_ptr<std::stringstream> source = buildSimple("(Float)","(2.0)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleOneBool() {
    shared_ptr<std::stringstream> source = buildSimple("(Bool)","(True)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleOneChar() {
    shared_ptr<std::stringstream> source = buildSimple("(Char)","('c')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleOneString() {
    shared_ptr<std::stringstream> source = buildSimple("(String)","(\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::tupleTwoInt() {
    shared_ptr<std::stringstream> source = buildSimple("(Int,Int)","(2,2)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleTwoFloat() {
    shared_ptr<std::stringstream> source = buildSimple("(Float,Float)","(2.0,2.0)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleTwoBool() {
    shared_ptr<std::stringstream> source = buildSimple("(Bool,Bool)","(True,False)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleTwoChar() {
    shared_ptr<std::stringstream> source = buildSimple("(Char,Char)","('c','h')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleTwoString() {
    shared_ptr<std::stringstream> source = buildSimple("(String,String)","(\"string\",\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleThreeInt() {
    shared_ptr<std::stringstream> source = buildSimple("(Int,Int,Int)","(1,2,3)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleThreeFloat() {
    shared_ptr<std::stringstream> source = buildSimple("(Float,Float,Float)","(1.0,2.0,3.0.)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleThreeBool() {
    shared_ptr<std::stringstream> source = buildSimple("(Bool,Bool,Bool)","(True,False,True)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleThreeChar() {
    shared_ptr<std::stringstream> source = buildSimple("(Char,Char,Char)","('c','h','a')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleThreeString() {
    shared_ptr<std::stringstream> source = buildSimple("(String,String,String)","(\"string\",\"string\",\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleMixed() {
    shared_ptr<std::stringstream> source = buildSimple("(Int,Float,Bool,Char,Char,Char,Char,String)","(1,2.0,True,'c','h','a','r',\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleListInt() {
    shared_ptr<std::stringstream> source = buildSimple("([Int],[Int])","([1],[1,2])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleListFloat() {
    shared_ptr<std::stringstream> source = buildSimple("([Float],[Float])","([1.0],[1.0,2.0])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleListBool() {
    shared_ptr<std::stringstream> source = buildSimple("([Bool],[Bool])","([True],[True,False])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleListChar() {
    shared_ptr<std::stringstream> source = buildSimple("([Char],[Char])","(['c'],['h','a'])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleListString() {
    shared_ptr<std::stringstream> source = buildSimple("([String],[String])","([\"string\"],[\"string\",\"string\"])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleNested() {
    shared_ptr<std::stringstream> source = buildSimple("((Int,Int),(Int,Int))","((1,2),(3,4))");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleTwoNested() {
    shared_ptr<std::stringstream> source = buildSimple("((Int,Float),(Bool,(Char,String)))","((1,2.0),(True,('c',\"har\")))");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleSuperNested() {
    shared_ptr<std::stringstream> source = buildSimple("(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,Char)))))))))))","('h',('e',('l',('l',('o',(' ',('w',('o',('r',('l',('d','!')))))))))))");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::tupleWrongType() {
    shared_ptr<std::stringstream> source = buildSimple("(Int,Float,Bool,Char,String)","(1.2,True,'c',\"string\",1)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

/*
 * Function
 */

void Test::funcRetInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Int","","func()",
                                              "Int","","2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcRetFloat() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Float", "", "func()",
                                              "Float", "", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcRetBool() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Bool", "", "func()",
                                              "Bool", "", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcRetChar() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Char", "", "func()",
                                              "Char", "", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcRetString() {
    shared_ptr<std::stringstream> source = buildMultiFunc("String", "", "func()",
                                              "String", "", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcIntRetInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Int","","func(2)",
                                              "Int->Int","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcFloatRetFloat() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Float","","func(2.0)",
                                              "Float->Float","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcBoolRetBool() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Bool","","func(True)",
                                              "Bool->Bool","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcCharRetChar() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Char","","func('c')",
                                              "Char->Char","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcStringRetString() {
    shared_ptr<std::stringstream> source = buildMultiFunc("String","","func(\"string\")",
                                              "String->String","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcIntIntRetInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Int","","func(2,2)",
                                              "Int->Int->Int","m n","m+n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcFloatFloatRetFloat() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Float","","func(2.0,2.0)",
                                              "Float->Float->Float","m n","m+n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcBoolBoolRetBool() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Bool","","func(True,False)",
                                              "Bool->Bool->Bool","m n","m||n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcCharCharRetChar() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Char","","func('c','c')",
                                              "Char->Char->Char","m n","'h'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcStringStringRetString() {
    shared_ptr<std::stringstream> source = buildMultiFunc("String","","func(\"string\",\"string\")",
                                              "String->String->String","m n","\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcListIntRetListInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("[Int]","","func([1])",
                                              "[Int]->[Int]","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcListFloatRetListFloat() {
    shared_ptr<std::stringstream> source = buildMultiFunc("[Float]","","func([1.0])",
                                              "[Float]->[Float]","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcListBoolRetListBool() {
    shared_ptr<std::stringstream> source = buildMultiFunc("[Bool]","","func([True])",
                                              "[Bool]->[Bool]","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcListCharRetListChar() {
    shared_ptr<std::stringstream> source = buildMultiFunc("[Char]","","func(['c','h','a','r'])",
                                              "[Char]->[Char]","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcListStringRetListString() {
    shared_ptr<std::stringstream> source = buildMultiFunc("[String]","","func([\"string\"])",
                                              "[String]->[String]","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcTupleIntIntRetTupleIntInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("(Int,Int)","","func((2,2))",
                                              "(Int,Int)->(Int,Int)","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcTupleFloatFloatRetTupleFloatFloat() {
    shared_ptr<std::stringstream> source = buildMultiFunc("(Float,Float)","","func((2.0,2.0))",
                                              "(Float,Float)->(Float,Float)","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcTupleBoolBoolRetTupleBoolBool() {
    shared_ptr<std::stringstream> source = buildMultiFunc("(Bool,Bool)","","func((True,False))",
                                              "(Bool,Bool)->(Bool,Bool)","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcTupleCharCharRetTupleCharChar() {
    shared_ptr<std::stringstream> source = buildMultiFunc("(Char,Char)","","func(('c','h'))",
                                              "(Char,Char)->(Char,Char)","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcTupleStringStringRetTupleStringString() {
    shared_ptr<std::stringstream> source = buildMultiFunc("(String,String)","","func((\"string\",\"string\"))",
                                              "(String,String)->(String,String)","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcIntToIntRetInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Int", "", "func1(func2)",
                                              "(Int->Int)->Int", "f", "f(2)",
                                              "Int->Int", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcFloatToFloatRetFloat() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Float", "", "func1(func2)",
                                              "(Float->Float)->Float", "f", "f(2.0)",
                                              "Float->Float", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcBoolToBoolRetBool() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Bool", "", "func1(func2)",
                                              "(Bool->Bool)->Bool", "f", "f(True)",
                                              "Bool->Bool", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcCharToCharRetChar() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Char", "", "func1(func2)",
                                              "(Char->Char)->Char", "f", "f('c')",
                                              "Char->Char", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcStringToStringRetString() {
    shared_ptr<std::stringstream> source = buildMultiFunc("String", "", "func1(func2)",
                                              "(String->String)->String", "f", "f(\"string\")",
                                              "String->String", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcListIntToListIntRetListInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("[Int]", "", "func1(func2)",
                                              "([Int]->[Int])->[Int]", "f", "f([1])",
                                              "[Int]->[Int]", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcTupleIntIntToTupleIntIntRetInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("(Int,Int)", "", "func1(func2)",
                                              "((Int,Int)->(Int,Int))->(Int,Int)", "f", "f((1,2))",
                                              "(Int,Int)->(Int,Int)", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

void Test::funcIntToIntRetToIntRetToInt() {
    shared_ptr<std::stringstream> source = buildMultiFunc("Int","","func1(func2)",
                                              "((Int->Int)->Int)->Int","f","f(func3)",
                                              "(Int->Int)->Int","f","f(2)",
                                              "Int->Int","n","n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == true);
}

/*
 * Keyword Test
 */

void Test::keywordInt() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Int->Int" << endl;
    *source << buildCase("Int",
              "2");

    std::cout << source->str() << endl;
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordFloat() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Float->Int" << endl;
    *source << buildCase("Float",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordBool() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Bool->Int" << endl;
    *source << buildCase("Bool",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordChar() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Char->Int" << endl;
    *source << buildCase("Char",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordString() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : String->Int" << endl;
    *source << buildCase("String",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordTrue() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Int->Int" << endl;
    *source << buildCase("True",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordFalse() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Int->Int" << endl;
    *source << buildCase("False",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordDef() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Int->Int" << endl;
    *source << buildCase("def",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

void Test::keywordMain() {
    shared_ptr<std::stringstream> source = make_shared<std::stringstream>();
    *source << "def main : Int->Int" << endl;
    *source << buildCase("main",
              "2");

    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT(compStatus == false);
}

// Single line comment test

void Test::comment() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "2", "+", "2 # This is a comment");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT (compStatus == true);
}

/*
* COMPILER TEST
* TODO: List
* TODO: Tuple
* TODO: Case & Pattern - Partially done
* TODO: Function Calls
*
* CODE TEST
* TODO: Test if generated code perform correct calculations
*/