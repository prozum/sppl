#include "TestMaster.h"

void TestMaster::setUp() {
    // First test setup
}

void TestMaster::tearDown() {
    // Final test cleanup
}

bool TestMaster::compileChecker(std::stringstream *source) {
    ostream out(0);
    ostream hout(0);
    int status;

    try {
        compiler::Compiler compiler(source, &out, &hout);
        compiler.set_backend(backend);
        status = compiler.compile();
    }
    catch (...) {
        return false;
    }

    if (status != 0) {
        return false;
    } else {
        return true;
    }
}

std::stringstream TestMaster::buildSimple(std::string signature,
                                          std::string body) {
    std::stringstream source;
    source << "def main : " << signature << endl << "| = " << body << endl;
    return source;
}

std::stringstream TestMaster::buildSimple(std::string signature,
                                          std::string pattern,
                                          std::string body) {
    std::stringstream source;
    source << "def main : " << signature << endl << "| " << pattern << " = " << body << endl;
    return source;
}

std::stringstream TestMaster::buildSimple(std::string pattern,
                             std::string left,
                             std::string op,
                             std::string right) {
    std::stringstream source;
    source << "def main : " << pattern << " | = " << left << " " << op << " " << right;
    return source;
}

std::stringstream *TestMaster::buildCase(std::stringstream *source,
                                         std::string pattern,
                                         std::string body) {
    *source << "| " << pattern << " = " << body << endl;
    return source;
}
/*
std::stringstream *TestMaster::buildMultiCase(std::stringstream *source, std::vector<string> signature, std::string ret, std::vector<std::vector<string>> pattern, std::vector<string> cas) {

}
*/

std::stringstream TestMaster::buildMultiCase(string signature,
                                             std::string ret,
                                             std::vector<string> pattern,
                                             std::string cas) {
    std::stringstream source;
    source << "def main : " << signature << "->" << ret << endl;

    for(auto p : pattern) {
        buildCase(&source, p, cas);
    }
    
    return source;
}
std::stringstream TestMaster::buildMultiCase(std::vector<string> signature,
                                             std::string ret,
                                             std::string pattern,
                                             std::string cas) {
    std::stringstream source;

    source << "def main : ";
    for (auto s : signature) {
        source << s << "->";
    }
    source << ret << endl;

    buildCase(&source, pattern, cas);
    
    return source;
}

std::stringstream TestMaster::buildMultiFunc(std::string mainSig, 
                                             std::string mainPat, 
                                             std::string mainBody,
                                             std::string funcSig, 
                                             std::string funcPat, 
                                             std::string funcBody) {
    std::stringstream source;
    source << "def main : " << mainSig << endl << buildCase(&source, mainPat, mainBody);
    source << "def func : " << funcSig << endl << buildCase(&source, funcPat, funcBody);
    return source;
}

std::stringstream TestMaster::buildMultiFunc(std::string mainSig, std::string mainPat, std::string mainBody,
                                             std::string func1Sig, std::string func1Pat, std::string func1Body,
                                             std::string func2Sig, std::string func2Pat, std::string func2Body) {
    std::stringstream source;
    source << "def main : " << mainSig << endl << buildCase(&source, mainPat, mainBody);
    source << "def func1 : " << func1Sig << endl << buildCase(&source, func1Pat, func1Body);
    source << "def func2 : " << func2Sig << endl << buildCase(&source, func2Pat, func2Body);
    return source;
}

std::stringstream TestMaster::buildMultiFunc(std::string mainSig, std::string mainPat, std::string mainBody,
                                             std::string func1Sig, std::string func1Pat, std::string func1Body,
                                             std::string func2Sig, std::string func2Pat, std::string func2Body,
                                             std::string func3Sig, std::string func3Pat, std::string func3Body) {
    std::stringstream source;
    source << "def main : " << mainSig << endl << buildCase(&source, mainPat, mainBody);
    source << "def func1 : " << func1Sig << endl << buildCase(&source, func1Pat, func1Body);
    source << "def func2 : " << func2Sig << endl << buildCase(&source, func2Pat, func2Body);
    source << "def func3 : " << func3Sig << endl << buildCase(&source, func3Pat, func3Body);
    return source;
}

/*
 * ADDITION
 */

// Addition - Integer

void TestMaster::addIntPosPos() {
    std::stringstream source = buildSimple("Int", "2", "+", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntZeroZero() {
    std::stringstream source = buildSimple("Int", "0", "+", "0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntPosNeg() {
    std::stringstream source = buildSimple("Int", "2", "+", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntNegPos() {
    std::stringstream source = buildSimple("Int", "-2", "+", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntNegNeg() {
    std::stringstream source = buildSimple("Int", "-2", "+", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Addition - Float

void TestMaster::addFloatPosPos() {
    std::stringstream source = buildSimple("Float", "2.0", "+", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatZeroZero() {
    std::stringstream source = buildSimple("Float", "0.0", "+", "0.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatPosNeg() {
    std::stringstream source = buildSimple("Float", "2.0", "+", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatNegPos() {
    std::stringstream source = buildSimple("Float", "-2.0", "+", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatNegNeg() {
    std::stringstream source = buildSimple("Float", "-2.0", "+", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Addition - Boolean

void TestMaster::addBoolTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "+", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addBoolTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "+", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addBoolFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "+", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addBoolFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "+", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Addition - Character

void TestMaster::addCharStrStr() {
    std::stringstream source = buildSimple("String", "\"string\"", "+", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addCharStrChar() {
    std::stringstream source = buildSimple("String", "\"string\"", "+", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addCharCharStr() {
    std::stringstream source = buildSimple("Char", "'c'", "+", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addCharCharChar() {
    std::stringstream source = buildSimple("Char", "'c'", "+", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Addition - Mix

void TestMaster::addMixFloatInt() {
    std::stringstream source = buildSimple("Float", "2.0", "+", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addMixIntFloat() {
    std::stringstream source = buildSimple("Int", "2", "+", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * SUBTRACTION
 */

// Subtraction - Integer

void TestMaster::subIntPosPos() {
    std::stringstream source = buildSimple("Int", "2", "-", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntZeroZero() {
    std::stringstream source = buildSimple("Int", "0", "-", "0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntPosNeg() {
    std::stringstream source = buildSimple("Int", "2", "-", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntNegPos() {
    std::stringstream source = buildSimple("Int", "-2", "-", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntNegNeg() {
    std::stringstream source = buildSimple("Int", "-2", "-", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Subtraction - Float

void TestMaster::subFloatPosPos() {
    std::stringstream source = buildSimple("Float", "2.0", "-", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatZeroZero() {
    std::stringstream source = buildSimple("Float", "0.0", "-", "0.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatPosNeg() {
    std::stringstream source = buildSimple("Float", "2.0", "-", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatNegPos() {
    std::stringstream source = buildSimple("Float", "-2.0", "-", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatNegNeg() {
    std::stringstream source = buildSimple("Float", "-2.0", "-", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Subtraction - Boolean

void TestMaster::subBoolTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "-", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subBoolTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "-", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subBoolFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "-", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subBoolFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "-", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Subtraction - Character

void TestMaster::subCharStrStr() {
    std::stringstream source = buildSimple("String", "\"string\"", "-", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subCharStrChar() {
    std::stringstream source = buildSimple("String", "\"string\"", "-", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subCharCharStr() {
    std::stringstream source = buildSimple("Char", "'c'", "-", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subCharCharChar() {
    std::stringstream source = buildSimple("Char", "'c'", "-", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Subtraction - Mix

void TestMaster::subMixFloatInt() {
    std::stringstream source = buildSimple("Float", "2.0", "-", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subMixIntFloat() {
    std::stringstream source = buildSimple("Int", "2", "-", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * Multiplication
 */

// Multiplication - Integer

void TestMaster::mulIntPosPos() {
    std::stringstream source = buildSimple("Int", "2", "*", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntZeroZero() {
    std::stringstream source = buildSimple("Int", "0", "*", "0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntPosNeg() {
    std::stringstream source = buildSimple("Int", "2", "*", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntNegPos() {
    std::stringstream source = buildSimple("Int", "-2", "*", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntNegNeg() {
    std::stringstream source = buildSimple("Int", "-2", "*", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Multiplication - Float

void TestMaster::mulFloatPosPos() {
    std::stringstream source = buildSimple("Float", "2.0", "*", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatZeroZero() {
    std::stringstream source = buildSimple("Float", "0.0", "*", "0.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatPosNeg() {
    std::stringstream source = buildSimple("Float", "2.0", "*", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatNegPos() {
    std::stringstream source = buildSimple("Float", "-2.0", "*", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatNegNeg() {
    std::stringstream source = buildSimple("Float", "-2.0", "*", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Multiplication - Boolean

void TestMaster::mulBoolTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "*", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulBoolTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "*", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulBoolFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "*", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulBoolFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "*", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Multiplication - Character

void TestMaster::mulCharStrStr() {
    std::stringstream source = buildSimple("String", "\"string\"", "*", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulCharStrChar() {
    std::stringstream source = buildSimple("String", "\"string\"", "*", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::mulCharCharStr() {
    std::stringstream source = buildSimple("Char", "'c'", "*", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::mulCharCharChar() {
    std::stringstream source = buildSimple("Char", "'c'", "*", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

// Multiplication - Mix

void TestMaster::mulMixFloatInt() {
    std::stringstream source = buildSimple("Float", "2.0", "*", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulMixIntFloat() {
    std::stringstream source = buildSimple("Int", "2", "*", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * Division
 */

// Division - Integer

void TestMaster::divIntPosPos() {
    std::stringstream source = buildSimple("Int", "2", "/", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntZeroZero() {
    std::stringstream source = buildSimple("Int", "0", "/", "0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntPosNeg() {
    std::stringstream source = buildSimple("Int", "2", "/", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntNegPos() {
    std::stringstream source = buildSimple("Int", "-2", "/", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntNegNeg() {
    std::stringstream source = buildSimple("Int", "-2", "/", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Division - Float

void TestMaster::divFloatPosPos() {
    std::stringstream source = buildSimple("Float", "2.0", "/", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatZeroZero() {
    std::stringstream source = buildSimple("Float", "0.0", "/", "0.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatPosNeg() {
    std::stringstream source = buildSimple("Float", "2.0", "/", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatNegPos() {
    std::stringstream source = buildSimple("Float", "-2.0", "/", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatNegNeg() {
    std::stringstream source = buildSimple("Float", "-2.0", "/", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Division - Boolean

void TestMaster::divBoolTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "/", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divBoolTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "/", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divBoolFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "/", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divBoolFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "/", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Division - Character

void TestMaster::divCharStrStr() {
    std::stringstream source = buildSimple("String", "\"string\"", "/", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divCharStrChar() {
    std::stringstream source = buildSimple("String", "\"string\"", "/", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::divCharCharStr() {
    std::stringstream source = buildSimple("Char", "'c'", "/", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::divCharCharChar() {
    std::stringstream source = buildSimple("Char", "'c'", "/", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

// Division - Mix

void TestMaster::divMixFloatInt() {
    std::stringstream source = buildSimple("Float", "2.0", "/", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divMixIntFloat() {
    std::stringstream source = buildSimple("Int", "2", "/", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * Modulo
 */

// Modulo - Integer

void TestMaster::modIntPosPos() {
    std::stringstream source = buildSimple("Int", "2", "%", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntZeroZero() {
    std::stringstream source = buildSimple("Int", "0", "%", "0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntPosNeg() {
    std::stringstream source = buildSimple("Int", "2", "%", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntNegPos() {
    std::stringstream source = buildSimple("Int", "-2", "%", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntNegNeg() {
    std::stringstream source = buildSimple("Int", "-2", "%", "-2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Modulo - Float

void TestMaster::modFloatPosPos() {
    std::stringstream source = buildSimple("Float", "2.0", "%", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatZeroZero() {
    std::stringstream source = buildSimple("Float", "0.0", "%", "0.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatPosNeg() {
    std::stringstream source = buildSimple("Float", "2.0", "%", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatNegPos() {
    std::stringstream source = buildSimple("Float", "-2.0", "%", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatNegNeg() {
    std::stringstream source = buildSimple("Float", "-2.0", "%", "-2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Modulo - Boolean

void TestMaster::modBoolTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "%", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modBoolTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "%", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modBoolFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "%", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modBoolFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "%", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Modulo - Character

void TestMaster::modCharStrStr() {
    std::stringstream source = buildSimple("String", "\"string\"", "%", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modCharStrChar() {
    std::stringstream source = buildSimple("String", "\"string\"", "%", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::modCharCharStr() {
    std::stringstream source = buildSimple("Char", "'c'", "%", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::modCharCharChar() {
    std::stringstream source = buildSimple("Char", "'c'", "%", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);

}

// Modulo - Mix

void TestMaster::modMixFloatInt() {
    std::stringstream source = buildSimple("Float", "2.0", "%", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modMixIntFloat() {
    std::stringstream source = buildSimple("Int", "2", "%", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * Binary/Bool Operators
 */

// Binary/Bool - Equal

void TestMaster::binEqualTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "==", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binEqualTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "==", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binEqualFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "==", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binEqualFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "==", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - NotEqual

void TestMaster::binNotEqualTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "!=", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "!=", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "!=", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "!=", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - And

void TestMaster::binAndTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "&&", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "&&", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "&&", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "&&", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - Or

void TestMaster::binOrTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "||", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binOrTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "||", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binOrFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "||", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binOrFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "||", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - GrEq

void TestMaster::binGrEqTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", ">=", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGrEqTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", ">=", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGrEqFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", ">=", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGrEqFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", ">=", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Binary/Bool - LeEq

void TestMaster::binLeEqTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "<=", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "<=", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "<=", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "<=", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Binary/Bool - Great

void TestMaster::binGreatTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", ">", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGreatTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", ">", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGreatFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", ">", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGreatFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", ">", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Binary/Bool - Less

void TestMaster::binLessTrueTrue() {
    std::stringstream source = buildSimple("Bool", "True", "<", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLessTrueFalse() {
    std::stringstream source = buildSimple("Bool", "True", "<", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLessFalseTrue() {
    std::stringstream source = buildSimple("Bool", "False", "<", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLessFalseFalse() {
    std::stringstream source = buildSimple("Bool", "False", "<", "False");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * Binary/Int
 */

// Binary/Int

void TestMaster::binEqualInt() {
    std::stringstream source = buildSimple("Bool", "2", "==", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualInt() {
    std::stringstream source = buildSimple("Bool", "2", "!=", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndInt() {
    std::stringstream source = buildSimple("Bool", "2", "&&", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binOrInt() {
    std::stringstream source = buildSimple("Bool", "2", "||", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqInt() {
    std::stringstream source = buildSimple("Bool", "2", "<=", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGrEqInt() {
    std::stringstream source = buildSimple("Bool", "2", ">=", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binLessInt() {
    std::stringstream source = buildSimple("Bool", "2", "<", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGreatInt() {
    std::stringstream source = buildSimple("Bool", "2", ">", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Binary/Float

void TestMaster::binEqualFloat() {
    std::stringstream source = buildSimple("Bool", "2", "==", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualFloat() {
    std::stringstream source = buildSimple("Bool", "2", "!=", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndFloat() {
    std::stringstream source = buildSimple("Bool", "2", "&&", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binOrFloat() {
    std::stringstream source = buildSimple("Bool", "2", "||", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqFloat() {
    std::stringstream source = buildSimple("Bool", "2", "<=", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGrEqFloat() {
    std::stringstream source = buildSimple("Bool", "2", ">=", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binLessFloat() {
    std::stringstream source = buildSimple("Bool", "2", "<", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGreatFloat() {
    std::stringstream source = buildSimple("Bool", "2", ">", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

/*
 * Case & Signature test
 */

// SIgnature length

void TestMaster::sigLengthZeroInt() {
    std::vector<string> signature;

    std::stringstream source = buildMultiCase(signature, "Int", "", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthOneInt() {
    std::vector<string> signature;
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthTwoInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthThreeInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthFourInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthFiveInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthSixInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthSevenInt() {
    std::vector<string> signature;
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthEightInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g h", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthNineInt() {
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

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g h i", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthTenInt() {
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

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g h i j", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Pattern Match

void TestMaster::patLengthSameInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::patLengthLessInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::patLengthMoreInt() {
    std::vector<string> signature;

    signature.push_back("Int");
    signature.push_back("Int");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Case Count

void TestMaster::casOneCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    string prog = source.str();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casTwoCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casThreeCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casFourCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casFiveCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casSixCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casSevenCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casEightCaseInt() {
    std::vector<string> pattern;
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    pattern.push_back("8");

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casNineCaseInt() {
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

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::casTenCaseInt() {
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

    std::stringstream source = buildMultiCase("Int","Int",pattern, "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

/*
 * List
 */

void TestMaster::listCasEmpty() {
    std::stringstream source = buildSimple("[Int]->Int", "[]", "2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listAppendEmpty() {
    std::stringstream source = buildSimple("[Int]", "1:[]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listInt() {
    std::stringstream source = buildSimple("[Int]", "[2]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listFloat() {
    std::stringstream source = buildSimple("[Float]", "[2.0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listBool() {
    std::stringstream source = buildSimple("[Bool]", "[True]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listChar() {
    std::stringstream source = buildSimple("[Char", "['c']");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listString() {
    std::stringstream source = buildSimple("[String]", "[\"string\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listListInt() {
    std::stringstream source = buildSimple("[[Int]]", "[[2]]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listListFloat() {
    std::stringstream source = buildSimple("[[Float]]", "[[2.0]]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listListBool() {
    std::stringstream source = buildSimple("[[Bool]]", "[[True]]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listListChar() {
    std::stringstream source = buildSimple("[[Char]]", "[['c']]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listListString() {
    std::stringstream source = buildSimple("[[String]]", "[[\"string\"]]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listListListInt() {
    std::stringstream source = buildSimple("[[[Int]]]", "[[[2]]]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listAppendInt() {
    std::stringstream source = buildSimple("[Int]", "1:[2,3]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listAppendFloat() {
    std::stringstream source = buildSimple("[Float]", "1.0:[2.0,3.0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listAppendBool() {
    std::stringstream source = buildSimple("[Bool]","True:[True,False]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listAppendChar() {
    std::stringstream source = buildSimple("[Char]", "'c':['h','a','r']");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listAppendString() {
    std::stringstream source = buildSimple("[String]", "\"String\":[\"Int\",\"Float\",\"Bool\",\"Char\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listLongInt() {
    std::stringstream source = buildSimple("[Int]","[0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listFloatInInt() {
    std::stringstream source = buildSimple("[Int]","[2.0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listBoolInInt() {
    std::stringstream source = buildSimple("[Int]","[True]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listCharInInt() {
    std::stringstream source = buildSimple("[Int]","['c']");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listStringInInt() {
    std::stringstream source = buildSimple("[Int]","[\"string\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listIntInFloat() {
    std::stringstream source = buildSimple("[Float]","[2]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listBoolInFloat() {
    std::stringstream source = buildSimple("[Float]","[True]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listCharInFloat() {
    std::stringstream source = buildSimple("[Float]","['c']");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listStringInFloat() {
    std::stringstream source = buildSimple("[Float]","[\"string\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listIntInBool() {
    std::stringstream source = buildSimple("[Bool]","[2]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listFloatInBool() {
    std::stringstream source = buildSimple("[Bool]","[2.0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listCharInBool() {
    std::stringstream source = buildSimple("[Bool]","['c']");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listStringInBool() {
    std::stringstream source = buildSimple("[Bool]","[\"string\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listIntInChar() {
    std::stringstream source = buildSimple("[Char]","[2]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listFloatInChar() {
    std::stringstream source = buildSimple("[Char]","[2.0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listBoolInChar() {
    std::stringstream source = buildSimple("[Char]","[True]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listStringInChar() {
    std::stringstream source = buildSimple("[Char]","[\"string\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listIntInString() {
    std::stringstream source = buildSimple("[String]","[2]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listFloatInString() {
    std::stringstream source = buildSimple("[String]","[2.0]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listBoolInString() {
    std::stringstream source = buildSimple("[String]","[True]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listCharInString() {
    std::stringstream source = buildSimple("[String]","['c']");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listMixTypes() {
    std::stringstream source = buildSimple("[Int]", "[2,2.0,False,'c',\"string\"]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::listNestedDifLengthInt() {
    std::stringstream source = buildSimple("[[Int]]","[[1,2,3],[1,2,3,4,5,6,7,8,9,0]]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listTupleInt() {
    std::stringstream source = buildSimple("[(Int,Int)]","[(1,2),(3,4),(5,6)]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listTupleFloat() {
    std::stringstream source = buildSimple("[(Float,Float)]","[(1.0,2.0),(3.0,4.0),(5.0,6.0)]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listTupleBool() {
    std::stringstream source = buildSimple("[(Bool,Bool)]","[(True,True),(True,False),(False,True),(False,False)]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listTupleChar() {
    std::stringstream source = buildSimple("[(Char,Char)]","[('c','h'),('a','r')]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::listTupleString() {
    std::stringstream source = buildSimple("[(String,String)]","[(\"string\",\"string\")]");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}


/*
 * Tuple
 */

void TestMaster::tupleEmpty() {
    std::stringstream source = buildSimple("()","()");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleEmptyInt() {
    std::stringstream source = buildSimple("(Int)","()");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleEmptyFloat() {
    std::stringstream source = buildSimple("(Float)","()");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleEmptyBool() {
    std::stringstream source = buildSimple("(Bool)","()");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleEmptyChar() {
    std::stringstream source = buildSimple("(Char)","()");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleEmptyString() {
    std::stringstream source = buildSimple("(String)","()");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleOneInt() {
    std::stringstream source = buildSimple("(Int)","(2)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleOneFloat() {
    std::stringstream source = buildSimple("(Float)","(2.0)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleOneBool() {
    std::stringstream source = buildSimple("(Bool)","(True)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleOneChar() {
    std::stringstream source = buildSimple("(Char)","('c')");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleOneString() {
    std::stringstream source = buildSimple("(String)","(\"string\")");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::tupleTwoInt() {
    std::stringstream source = buildSimple("(Int,Int)","(2,2)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleTwoFloat() {
    std::stringstream source = buildSimple("(Float,Float)","(2.0,2.0)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleTwoBool() {
    std::stringstream source = buildSimple("(Bool,Bool)","(True,False)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleTwoChar() {
    std::stringstream source = buildSimple("(Char,Char)","('c','h')");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleTwoString() {
    std::stringstream source = buildSimple("(String,String)","(\"string\",\"string\")");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleThreeInt() {
    std::stringstream source = buildSimple("(Int,Int,Int)","(1,2,3)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleThreeFloat() {
    std::stringstream source = buildSimple("(Float,Float,Float)","(1.0,2.0,3.0.)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleThreeBool() {
    std::stringstream source = buildSimple("(Bool,Bool,Bool)","(True,False,True)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleThreeChar() {
    std::stringstream source = buildSimple("(Char,Char,Char)","('c','h','a')");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleThreeString() {
    std::stringstream source = buildSimple("(String,String,String)","(\"string\",\"string\",\"string\")");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleMixed() {
    std::stringstream source = buildSimple("(Int,Float,Bool,Char,Char,Char,Char,String)","(1,2.0,True,'c','h','a','r',\"string\")");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleListInt() {
    std::stringstream source = buildSimple("([Int],[Int])","([1],[1,2])");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleListFloat() {
    std::stringstream source = buildSimple("([Float],[Float])","([1.0],[1.0,2.0])");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleListBool() {
    std::stringstream source = buildSimple("([Bool],[Bool])","([True],[True,False])");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleListChar() {
    std::stringstream source = buildSimple("([Char],[Char])","(['c'],['h','a'])");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleListString() {
    std::stringstream source = buildSimple("([String],[String])","([\"string\"],[\"string\",\"string\"])");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleNested() {
    std::stringstream source = buildSimple("((Int,Int),(Int,Int))","((1,2),(3,4))");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleTwoNested() {
    std::stringstream source = buildSimple("((Int,Float),(Bool,(Char,String)))","((1,2.0),(True,('c',\"har\")))");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleSuperNested() {
    std::stringstream source = buildSimple("(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,Char)))))))))))","('h',('e',('l',('l',('o',(' ',('w',('o',('r',('l',('d','!')))))))))))");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::tupleWrongType() {
    std::stringstream source = buildSimple("(Int,Float,Bool,Char,String)","(1.2,True,'c',\"string\",1)");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

/*
 * Function
 */

void TestMaster::funcRetInt() {
    std::stringstream source = buildMultiFunc("Int","","func()",
                                              "Int","","2");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcRetFloat() {
    std::stringstream source = buildMultiFunc("Float", "", "func()", 
                                              "Float", "", "2.0");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcRetBool() {
    std::stringstream source = buildMultiFunc("Bool", "", "func()", 
                                              "Bool", "", "True");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcRetChar() {
    std::stringstream source = buildMultiFunc("Char", "", "func()", 
                                              "Char", "", "'c'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcRetString() {
    std::stringstream source = buildMultiFunc("String", "", "func()", 
                                              "String", "", "\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcIntRetInt() {
    std::stringstream source = buildMultiFunc("Int","","func(2)",
                                              "Int->Int","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcFloatRetFloat() {
    std::stringstream source = buildMultiFunc("Float","","func(2.0)",
                                              "Float->Float","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcBoolRetBool() {
    std::stringstream source = buildMultiFunc("Bool","","func(True)",
                                              "Bool->Bool","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcCharRetChar() {
    std::stringstream source = buildMultiFunc("Char","","func('c')",
                                              "Char->Char","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcStringRetString() {
    std::stringstream source = buildMultiFunc("String","","func(\"string\")",
                                              "String->String","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcIntIntRetInt() {
    std::stringstream source = buildMultiFunc("Int","","func(2,2)",
                                              "Int->Int->Int","m n","m+n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcFloatFloatRetFloat() {
    std::stringstream source = buildMultiFunc("Float","","func(2.0,2.0)",
                                              "Float->Float->Float","m n","m+n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcBoolBoolRetBool() {
    std::stringstream source = buildMultiFunc("Bool","","func(True,False)",
                                              "Bool->Bool->Bool","m n","m||n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcCharCharRetChar() {
    std::stringstream source = buildMultiFunc("Char","","func('c','c')",
                                              "Char->Char->Char","m n","'h'");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcStringStringRetString() {
    std::stringstream source = buildMultiFunc("String","","func(\"string\",\"string\")",
                                              "String->String->String","m n","\"string\"");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcListIntRetListInt() {
    std::stringstream source = buildMultiFunc("[Int]","","func([1])",
                                              "[Int]->[Int]","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcListFloatRetListFloat() {
    std::stringstream source = buildMultiFunc("[Float]","","func([1.0])",
                                              "[Float]->[Float]","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcListBoolRetListBool() {
    std::stringstream source = buildMultiFunc("[Bool]","","func([True])",
                                              "[Bool]->[Bool]","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcListCharRetListChar() {
    std::stringstream source = buildMultiFunc("[Char]","","func(['c','h','a','r'])",
                                              "[Char]->[Char]","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcListStringRetListString() {
    std::stringstream source = buildMultiFunc("[String]","","func([\"string\"])",
                                              "[String]->[String]","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcTupleIntIntRetTupleIntInt() {
    std::stringstream source = buildMultiFunc("(Int,Int)","","func((2,2))",
                                              "(Int,Int)->(Int,Int)","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcTupleFloatFloatRetTupleFloatFloat() {
    std::stringstream source = buildMultiFunc("(Float,Float)","","func((2.0,2.0))",
                                              "(Float,Float)->(Float,Float)","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcTupleBoolBoolRetTupleBoolBool() {
    std::stringstream source = buildMultiFunc("(Bool,Bool)","","func((True,False))",
                                              "(Bool,Bool)->(Bool,Bool)","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcTupleCharCharRetTupleCharChar() {
    std::stringstream source = buildMultiFunc("(Char,Char)","","func(('c','h'))",
                                              "(Char,Char)->(Char,Char)","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcTupleStringStringRetTupleStringString() {
    std::stringstream source = buildMultiFunc("(String,String)","","func((\"string\",\"string\"))",
                                              "(String,String)->(String,String)","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcIntToIntRetInt() {
    std::stringstream source = buildMultiFunc("Int", "", "func1(func2)",
                                              "(Int->Int)->Int", "f", "f(2)",
                                              "Int->Int", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcFloatToFloatRetFloat() {
    std::stringstream source = buildMultiFunc("Float", "", "func1(func2)",
                                              "(Float->Float)->Float", "f", "f(2.0)",
                                              "Float->Float", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcBoolToBoolRetBool() {
    std::stringstream source = buildMultiFunc("Bool", "", "func1(func2)",
                                              "(Bool->Bool)->Bool", "f", "f(True)",
                                              "Bool->Bool", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcCharToCharRetChar() {
    std::stringstream source = buildMultiFunc("Char", "", "func1(func2)",
                                              "(Char->Char)->Char", "f", "f('c')",
                                              "Char->Char", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcStringToStringRetString() {
    std::stringstream source = buildMultiFunc("String", "", "func1(func2)",
                                              "(String->String)->String", "f", "f(\"string\")",
                                              "String->String", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcListIntToListIntRetListInt() {
    std::stringstream source = buildMultiFunc("[Int]", "", "func1(func2)",
                                              "([Int]->[Int])->[Int]", "f", "f([1])",
                                              "[Int]->[Int]", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcTupleIntIntToTupleIntIntRetInt() {
    std::stringstream source = buildMultiFunc("(Int,Int)", "", "func1(func2)",
                                              "((Int,Int)->(Int,Int))->(Int,Int)", "f", "f((1,2))",
                                              "(Int,Int)->(Int,Int)", "n", "n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::funcIntToIntRetToIntRetToInt() {
    std::stringstream source = buildMultiFunc("Int","","func1(func2)",
                                              "((Int->Int)->Int)->Int","f","f(func3)",
                                              "(Int->Int)->Int","f","f(2)",
                                              "Int->Int","n","n");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

/*
 * Keyword Test
 */

void TestMaster::keywordInt() {
    std::stringstream source;
    source << "def main : Int->Int" << endl;
    buildCase(&source, "Int", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordFloat() {
    std::stringstream source;
    source << "def main : Float->Int" << endl;
    buildCase(&source, "Float", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordBool() {
    std::stringstream source;
    source << "def main : Bool->Int" << endl;
    buildCase(&source, "Bool", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordChar() {
    std::stringstream source;
    source << "def main : Char->Int" << endl;
    buildCase(&source, "Char", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordString() {
    std::stringstream source;
    source << "def main : String->Int" << endl;
    buildCase(&source, "String", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordTrue() {
    std::stringstream source;
    source << "def main : Int->Int" << endl;
    buildCase(&source, "True", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordFalse() {
    std::stringstream source;
    source << "def main : Int->Int" << endl;
    buildCase(&source, "False", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordDef() {
    std::stringstream source;
    source << "def main : Int->Int" << endl;
    buildCase(&source, "def", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::keywordMain() {
    std::stringstream source;
    source << "def main : Int->Int" << endl;
    buildCase(&source, "main", "2");

    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

// Single line comment test

void TestMaster::comment() {
    std::stringstream source = buildSimple("Int", "2", "+", "2 # This is a comment");
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT (status == true);
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