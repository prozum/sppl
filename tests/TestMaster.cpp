#include "TestMaster.h"

void TestMaster::setUp() {
    // First test setup
}

void TestMaster::tearDown() {
    // Final test cleanup
}

void TestMaster::clearUp() {
    remove("source.sppl");
    remove("target.body");
    remove("target.header");
}

bool TestMaster::compileChecker() {
    in = ifstream("source.sppl");
    out = ofstream("target.body");
    hout = ofstream("target.header");

    try {
        compiler::Compiler compiler(&in, &out, &hout);
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

void TestMaster::buildSimple(std::string pattern,
                             std::string left,
                             std::string op,
                             std::string right) {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : " << pattern << " | = " << left << " " << op << " " << right;
    sourceFile.close();
}

/*
 * ADDITION
 */

// Addition - Integer

void TestMaster::addIntPosPos() {
    buildSimple("Int", "2", "+", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntZeroZero() {
    buildSimple("Int", "0", "+", "0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntPosNeg() {
    buildSimple("Int", "2", "+", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntNegPos() {
    buildSimple("Int", "-2", "+", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addIntNegNeg() {
    buildSimple("Int", "-2", "+", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Addition - Float

void TestMaster::addFloatPosPos() {
    buildSimple("Float", "2.0", "+", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatZeroZero() {
    buildSimple("Float", "0.0", "+", "0.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatPosNeg() {
    buildSimple("Float", "2.0", "+", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatNegPos() {
    buildSimple("Float", "-2.0", "+", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::addFloatNegNeg() {
    buildSimple("Float", "-2.0", "+", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Addition - Boolean

void TestMaster::addBoolTrueTrue() {
    buildSimple("Bool", "True", "+", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addBoolTrueFalse() {
    buildSimple("Bool", "True", "+", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addBoolFalseTrue() {
    buildSimple("Bool", "False", "+", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addBoolFalseFalse() {
    buildSimple("Bool", "False", "+", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Addition - Character

void TestMaster::addCharStrStr() {
    buildSimple("String", "\"string\"", "+", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addCharStrChar() {
    buildSimple("String", "\"string\"", "+", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addCharCharStr() {
    buildSimple("Char", "'c'", "+", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addCharCharChar() {
    buildSimple("Char", "'c'", "+", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Addition - Mix

void TestMaster::addMixFloatInt() {
    buildSimple("Float", "2.0", "+", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::addMixIntFloat() {
    buildSimple("Int", "2", "+", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

/*
 * SUBTRACTION
 */

// Subtraction - Integer

void TestMaster::subIntPosPos() {
    buildSimple("Int", "2", "-", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntZeroZero() {
    buildSimple("Int", "0", "-", "0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntPosNeg() {
    buildSimple("Int", "2", "-", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntNegPos() {
    buildSimple("Int", "-2", "-", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subIntNegNeg() {
    buildSimple("Int", "-2", "-", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Subtraction - Float

void TestMaster::subFloatPosPos() {
    buildSimple("Float", "2.0", "-", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatZeroZero() {
    buildSimple("Float", "0.0", "-", "0.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatPosNeg() {
    buildSimple("Float", "2.0", "-", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatNegPos() {
    buildSimple("Float", "-2.0", "-", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::subFloatNegNeg() {
    buildSimple("Float", "-2.0", "-", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Subtraction - Boolean

void TestMaster::subBoolTrueTrue() {
    buildSimple("Bool", "True", "-", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subBoolTrueFalse() {
    buildSimple("Bool", "True", "-", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subBoolFalseTrue() {
    buildSimple("Bool", "False", "-", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subBoolFalseFalse() {
    buildSimple("Bool", "False", "-", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Subtraction - Character

void TestMaster::subCharStrStr() {
    buildSimple("String", "\"string\"", "-", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subCharStrChar() {
    buildSimple("String", "\"string\"", "-", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subCharCharStr() {
    buildSimple("Char", "'c'", "-", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subCharCharChar() {
    buildSimple("Char", "'c'", "-", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Subtraction - Mix

void TestMaster::subMixFloatInt() {
    buildSimple("Float", "2.0", "-", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::subMixIntFloat() {
    buildSimple("Int", "2", "-", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

/*
 * Multiplication
 */

// Multiplication - Integer

void TestMaster::mulIntPosPos() {
    buildSimple("Int", "2", "*", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntZeroZero() {
    buildSimple("Int", "0", "*", "0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntPosNeg() {
    buildSimple("Int", "2", "*", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntNegPos() {
    buildSimple("Int", "-2", "*", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulIntNegNeg() {
    buildSimple("Int", "-2", "*", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Multiplication - Float

void TestMaster::mulFloatPosPos() {
    buildSimple("Float", "2.0", "*", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatZeroZero() {
    buildSimple("Float", "0.0", "*", "0.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatPosNeg() {
    buildSimple("Float", "2.0", "*", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatNegPos() {
    buildSimple("Float", "-2.0", "*", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::mulFloatNegNeg() {
    buildSimple("Float", "-2.0", "*", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Multiplication - Boolean

void TestMaster::mulBoolTrueTrue() {
    buildSimple("Bool", "True", "*", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulBoolTrueFalse() {
    buildSimple("Bool", "True", "*", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulBoolFalseTrue() {
    buildSimple("Bool", "False", "*", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulBoolFalseFalse() {
    buildSimple("Bool", "False", "*", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Multiplication - Character

void TestMaster::mulCharStrStr() {
    buildSimple("String", "\"string\"", "*", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulCharStrChar() {
    buildSimple("String", "\"string\"", "*", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::mulCharCharStr() {
    buildSimple("Char", "'c'", "*", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::mulCharCharChar() {
    buildSimple("Char", "'c'", "*", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

// Multiplication - Mix

void TestMaster::mulMixFloatInt() {
    buildSimple("Float", "2.0", "*", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::mulMixIntFloat() {
    buildSimple("Int", "2", "*", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

/*
 * Division
 */

// Division - Integer

void TestMaster::divIntPosPos() {
    buildSimple("Int", "2", "/", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntZeroZero() {
    buildSimple("Int", "0", "/", "0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntPosNeg() {
    buildSimple("Int", "2", "/", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntNegPos() {
    buildSimple("Int", "-2", "/", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divIntNegNeg() {
    buildSimple("Int", "-2", "/", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Division - Float

void TestMaster::divFloatPosPos() {
    buildSimple("Float", "2.0", "/", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatZeroZero() {
    buildSimple("Float", "0.0", "/", "0.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatPosNeg() {
    buildSimple("Float", "2.0", "/", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatNegPos() {
    buildSimple("Float", "-2.0", "/", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::divFloatNegNeg() {
    buildSimple("Float", "-2.0", "/", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Division - Boolean

void TestMaster::divBoolTrueTrue() {
    buildSimple("Bool", "True", "/", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divBoolTrueFalse() {
    buildSimple("Bool", "True", "/", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divBoolFalseTrue() {
    buildSimple("Bool", "False", "/", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divBoolFalseFalse() {
    buildSimple("Bool", "False", "/", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Division - Character

void TestMaster::divCharStrStr() {
    buildSimple("String", "\"string\"", "/", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divCharStrChar() {
    buildSimple("String", "\"string\"", "/", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::divCharCharStr() {
    buildSimple("Char", "'c'", "/", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::divCharCharChar() {
    buildSimple("Char", "'c'", "/", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

// Division - Mix

void TestMaster::divMixFloatInt() {
    buildSimple("Float", "2.0", "/", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::divMixIntFloat() {
    buildSimple("Int", "2", "/", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

/*
 * Modulo
 */

// Modulo - Integer

void TestMaster::modIntPosPos() {
    buildSimple("Int", "2", "%", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntZeroZero() {
    buildSimple("Int", "0", "%", "0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntPosNeg() {
    buildSimple("Int", "2", "%", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntNegPos() {
    buildSimple("Int", "-2", "%", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modIntNegNeg() {
    buildSimple("Int", "-2", "%", "-2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Modulo - Float

void TestMaster::modFloatPosPos() {
    buildSimple("Float", "2.0", "%", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatZeroZero() {
    buildSimple("Float", "0.0", "%", "0.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatPosNeg() {
    buildSimple("Float", "2.0", "%", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatNegPos() {
    buildSimple("Float", "-2.0", "%", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::modFloatNegNeg() {
    buildSimple("Float", "-2.0", "%", "-2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Modulo - Boolean

void TestMaster::modBoolTrueTrue() {
    buildSimple("Bool", "True", "%", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modBoolTrueFalse() {
    buildSimple("Bool", "True", "%", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modBoolFalseTrue() {
    buildSimple("Bool", "False", "%", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modBoolFalseFalse() {
    buildSimple("Bool", "False", "%", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Modulo - Character

void TestMaster::modCharStrStr() {
    buildSimple("String", "\"string\"", "%", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modCharStrChar() {
    buildSimple("String", "\"string\"", "%", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::modCharCharStr() {
    buildSimple("Char", "'c'", "%", "\"string\"");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

void TestMaster::modCharCharChar() {
    buildSimple("Char", "'c'", "%", "'c'");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);

}

// Modulo - Mix

void TestMaster::modMixFloatInt() {
    buildSimple("Float", "2.0", "%", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::modMixIntFloat() {
    buildSimple("Int", "2", "%", "2.0");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

/*
 * Binary/Bool Operators
 */

// Binary/Bool - Equal

void TestMaster::binEqualTrueTrue() {
    buildSimple("Bool", "True", "==", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binEqualTrueFalse() {
    buildSimple("Bool", "True", "==", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binEqualFalseTrue() {
    buildSimple("Bool", "False", "==", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binEqualFalseFalse() {
    buildSimple("Bool", "False", "==", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - NotEqual

void TestMaster::binNotEqualTrueTrue() {
    buildSimple("Bool", "True", "!=", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualTrueFalse() {
    buildSimple("Bool", "True", "!=", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualFalseTrue() {
    buildSimple("Bool", "False", "!=", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualFalseFalse() {
    buildSimple("Bool", "False", "!=", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - And

void TestMaster::binAndTrueTrue() {
    buildSimple("Bool", "True", "&&", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndTrueFalse() {
    buildSimple("Bool", "True", "&&", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndFalseTrue() {
    buildSimple("Bool", "False", "&&", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndFalseFalse() {
    buildSimple("Bool", "False", "&&", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - Or

void TestMaster::binOrTrueTrue() {
    buildSimple("Bool", "True", "||", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binOrTrueFalse() {
    buildSimple("Bool", "True", "||", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binOrFalseTrue() {
    buildSimple("Bool", "False", "||", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binOrFalseFalse() {
    buildSimple("Bool", "False", "||", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Binary/Bool - GrEq

void TestMaster::binGrEqTrueTrue() {
    buildSimple("Bool", "True", ">=", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGrEqTrueFalse() {
    buildSimple("Bool", "True", ">=", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGrEqFalseTrue() {
    buildSimple("Bool", "False", ">=", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGrEqFalseFalse() {
    buildSimple("Bool", "False", ">=", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Binary/Bool - LeEq

void TestMaster::binLeEqTrueTrue() {
    buildSimple("Bool", "True", "<=", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqTrueFalse() {
    buildSimple("Bool", "True", "<=", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqFalseTrue() {
    buildSimple("Bool", "False", "<=", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqFalseFalse() {
    buildSimple("Bool", "False", "<=", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Binary/Bool - Great

void TestMaster::binGreatTrueTrue() {
    buildSimple("Bool", "True", ">", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGreatTrueFalse() {
    buildSimple("Bool", "True", ">", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGreatFalseTrue() {
    buildSimple("Bool", "False", ">", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binGreatFalseFalse() {
    buildSimple("Bool", "False", ">", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

// Binary/Bool - Less

void TestMaster::binLessTrueTrue() {
    buildSimple("Bool", "True", "<", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLessTrueFalse() {
    buildSimple("Bool", "True", "<", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLessFalseTrue() {
    buildSimple("Bool", "False", "<", "True");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLessFalseFalse() {
    buildSimple("Bool", "False", "<", "False");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

/*
 * Binary/Int
 */

// Binary/Int

void TestMaster::binEqualInt() {
    buildSimple("Bool", "2", "==", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualInt() {
    buildSimple("Bool", "2", "!=", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndInt() {
    buildSimple("Bool", "2", "&&", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binOrInt() {
    buildSimple("Bool", "2", "||", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqInt() {
    buildSimple("Bool", "2", "<=", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGrEqInt() {
    buildSimple("Bool", "2", ">=", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binLessInt() {
    buildSimple("Bool", "2", "<", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGreatInt() {
    buildSimple("Bool", "2", ">", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Binary/Float

void TestMaster::binEqualFloat() {
    buildSimple("Bool", "2", "==", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binNotEqualFloat() {
    buildSimple("Bool", "2", "!=", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binAndFloat() {
    buildSimple("Bool", "2", "&&", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binOrFloat() {
    buildSimple("Bool", "2", "||", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == false);
}

void TestMaster::binLeEqFloat() {
    buildSimple("Bool", "2", "<=", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGrEqFloat() {
    buildSimple("Bool", "2", ">=", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binLessFloat() {
    buildSimple("Bool", "2", "<", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

void TestMaster::binGreatFloat() {
    buildSimple("Bool", "2", ">", "2");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
}

// Single line comment test

void TestMaster::comment() {
    buildSimple("Int", "2", "+", "2 # This is a comment");
    bool status = compileChecker();
    CPPUNIT_ASSERT(status == true);
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