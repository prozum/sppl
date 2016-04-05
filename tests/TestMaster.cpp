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

    try {
        compiler::Compiler compiler(source, &out, &hout);
        compiler.set_backend(backend);
        status = compiler.compile();
    }
    catch (...)
    {
        return false;
    }

    if (status != 0) {
        return false;
    } else {
        return true;
    }
}

std::stringstream TestMaster::buildSimple(std::string pattern,
                             std::string left,
                             std::string op,
                             std::string right) {
    std::stringstream source;
    source << "def main : " << pattern << " | = " << left << " " << op << " " << right;
    return source;
}

std::stringstream *TestMaster::buildCase(std::stringstream *source, std::string pattern, std::string body) {
    *source << "| " << pattern << " = " << body << "\n";
    return source;
}
/*
std::stringstream *TestMaster::buildMultiCase(std::stringstream *source, std::vector<string> signature, std::string ret, std::vector<std::vector<string>> pattern, std::vector<string> cas) {

}

std::stringstream *TestMaster::buildMultiCase(std::stringstream *source, std::vector<string> signature, std::string ret, std::vector<string> pattern, std::vector<string> cas) {

}
*/
std::stringstream TestMaster::buildMultiCase(std::vector<string> signature,
                                             std::string ret,
                                             std::string pattern,
                                             std::vector<string> cas) {
    std::stringstream source;

    source << "def main : ";
    for (auto s : signature) {
        source << s << "->";
    }
    source << ret << endl;

    for (auto c : cas) {
        buildCase(&source, pattern, c);
    }
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
    std::vector<string> cas;

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthOneInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthTwoInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthThreeInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthFourInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthFiveInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthSixInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthSevenInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthEightInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g h", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthNineInt() {

    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g h i", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::sigLengthTenInt() {

    std::vector<string> signature;
    std::vector<string> cas;

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

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c d e f g h i j", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

// Pattern Match

void TestMaster::patLengthSameInt() {
    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::patLengthLessInt() {
    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::patLengthMoreInt() {
    std::vector<string> signature;
    std::vector<string> cas;

    signature.push_back("Int");
    signature.push_back("Int");

    cas.push_back("2");

    std::stringstream source = buildMultiCase(signature, "Int", "a b c", cas);
    bool status = compileChecker(&source);
    CPPUNIT_ASSERT(status == false);
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
* TODO: Case & Pattern
* TODO: Function Calls
*
* CODE TEST
* TODO: Test if generated code perform correct calculations
*/