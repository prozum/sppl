#include "TestMaster.h"

void TestMaster::setUp() {

}

void TestMaster::tearDown() {
    remove("source.sppl");
    remove("target.body");
    remove("target.head");
    status = 0;
}

void TestMaster::compileChecker(bool success) {
    in = ifstream("source.sppl");
    out = ofstream("target.body");
    out = ofstream("target.header");

    try {
        compiler::Compiler compiler(&in, &out, &hout);
        compiler.set_backend(backend);
        status = compiler.compile();
    }
    catch (...)
    {
        CPPUNIT_ASSERT(false);  // Exceptions should never be thrown by the compiler
    }

    if ((status != 0) == success) {
        CPPUNIT_ASSERT(false);
    } else {
        CPPUNIT_ASSERT(true);
    }

    remove("source.sppl");
    remove("target.body");
    remove("target.head");
}

void TestMaster::buildSimple(std::string pattern, std::string left, std::string op, std::string right, bool status) {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : " << pattern << " | = " << left << " " << op << " " << right;
    sourceFile.close();
    compileChecker(status);
}

/*
 * ADDITION
 */

// Addition - Integer

void TestMaster::addIntPosPos() {
    buildSimple("Int", "2", "+", "2", true);
}

void TestMaster::addIntZeroZero() {
    buildSimple("Int", "0", "+", "0", true);
}

void TestMaster::addIntPosNeg() {
    buildSimple("Int", "2", "+", "-2", true);
}

void TestMaster::addIntNegPos() {
    buildSimple("Int", "-2", "+", "2", true);
}

void TestMaster::addIntNegNeg() {
    buildSimple("Int", "-2", "+", "-2", true);
}

// Addition - Float

void TestMaster::addFloatPosPos() {
    buildSimple("Float", "2.0", "+", "2.0", true);
}

void TestMaster::addFloatZeroZero() {
    buildSimple("Float", "0.0", "+", "0.0", true);
}

void TestMaster::addFloatPosNeg() {
    buildSimple("Float", "2.0", "+", "-2.0", true);
}

void TestMaster::addFloatNegPos() {
    buildSimple("Float", "-2.0", "+", "2.0", true);
}

void TestMaster::addFloatNegNeg() {
    buildSimple("Float", "-2.0", "+", "-2.0", true);
}

// Addition - Boolean

void TestMaster::addBoolTrueTrue() {
    buildSimple("Bool", "True", "+", "True", false);
}

void TestMaster::addBoolTrueFalse() {
    buildSimple("Bool", "True", "+", "False", false);
}

void TestMaster::addBoolFalseTrue() {
    buildSimple("Bool", "False", "+", "True", false);
}

void TestMaster::addBoolFalseFalse() {
    buildSimple("Bool", "False", "+", "False", false);
}

// Addition - Character

void TestMaster::addCharStrStr() {
    buildSimple("String", "\"string\"", "+", "\"string\"", false);
}

void TestMaster::addCharStrChar() {
    buildSimple("String", "\"string\"", "+", "'c'", false);

}

void TestMaster::addCharCharStr() {
    buildSimple("Char", "'c'", "+", "\"string\"", false);

}

void TestMaster::addCharCharChar() {
    buildSimple("Char", "'c'", "+", "'c'", false);

}

// Addition - Mix

void TestMaster::addMixFloatInt() {
    buildSimple("Float", "2.0", "+", "2", false);
}

void TestMaster::addMixIntFloat() {
    buildSimple("Int", "2", "+", "2.0", false);
}

/*
 * SUBTRACTION
 */

// Subtraction - Integer

void TestMaster::subIntPosPos() {
    buildSimple("Int", "2", "-", "2", true);
}

void TestMaster::subIntZeroZero() {
    buildSimple("Int", "0", "-", "0", true);
}

void TestMaster::subIntPosNeg() {
    buildSimple("Int", "2", "-", "-2", true);
}

void TestMaster::subIntNegPos() {
    buildSimple("Int", "-2", "-", "2", true);
}

void TestMaster::subIntNegNeg() {
    buildSimple("Int", "-2", "-", "-2", true);
}

// Subtraction - Float

void TestMaster::subFloatPosPos() {
    buildSimple("Float", "2.0", "-", "2.0", true);
}

void TestMaster::subFloatZeroZero() {
    buildSimple("Float", "0.0", "-", "0.0", true);
}

void TestMaster::subFloatPosNeg() {
    buildSimple("Float", "2.0", "-", "-2.0", true);
}

void TestMaster::subFloatNegPos() {
    buildSimple("Float", "-2.0", "-", "2.0", true);
}

void TestMaster::subFloatNegNeg() {
    buildSimple("Float", "-2.0", "-", "-2.0", true);
}

// Subtraction - Boolean

void TestMaster::subBoolTrueTrue() {
    buildSimple("Bool", "True", "-", "True", false);
}

void TestMaster::subBoolTrueFalse() {
    buildSimple("Bool", "True", "-", "False", false);
}

void TestMaster::subBoolFalseTrue() {
    buildSimple("Bool", "False", "-", "True", false);
}

void TestMaster::subBoolFalseFalse() {
    buildSimple("Bool", "False", "-", "False", false);
}

// Subtraction - Character

void TestMaster::subCharStrStr() {
    buildSimple("String", "\"string\"", "-", "\"string\"", false);
}

void TestMaster::subCharStrChar() {
    buildSimple("String", "\"string\"", "-", "'c'", false);

}

void TestMaster::subCharCharStr() {
    buildSimple("Char", "'c'", "-", "\"string\"", false);

}

void TestMaster::subCharCharChar() {
    buildSimple("Char", "'c'", "-", "'c'", false);

}

// Subtraction - Mix

void TestMaster::subMixFloatInt() {
    buildSimple("Float", "2.0", "-", "2", false);
}

void TestMaster::subMixIntFloat() {
    buildSimple("Int", "2", "-", "2.0", false);
}

/*
 * Multiplication
 */

// Multiplication - Integer

void TestMaster::mulIntPosPos() {
    buildSimple("Int", "2", "*", "2", true);
}

void TestMaster::mulIntZeroZero() {
    buildSimple("Int", "0", "*", "0", true);
}

void TestMaster::mulIntPosNeg() {
    buildSimple("Int", "2", "*", "-2", true);
}

void TestMaster::mulIntNegPos() {
    buildSimple("Int", "-2", "*", "2", true);
}

void TestMaster::mulIntNegNeg() {
    buildSimple("Int", "-2", "*", "-2", true);
}

// Multiplication - Float

void TestMaster::mulFloatPosPos() {
    buildSimple("Float", "2.0", "*", "2.0", true);
}

void TestMaster::mulFloatZeroZero() {
    buildSimple("Float", "0.0", "*", "0.0", true);
}

void TestMaster::mulFloatPosNeg() {
    buildSimple("Float", "2.0", "*", "-2.0", true);
}

void TestMaster::mulFloatNegPos() {
    buildSimple("Float", "-2.0", "*", "2.0", true);
}

void TestMaster::mulFloatNegNeg() {
    buildSimple("Float", "-2.0", "*", "-2.0", true);
}

// Multiplication - Boolean

void TestMaster::mulBoolTrueTrue() {
    buildSimple("Bool", "True", "*", "True", false);
}

void TestMaster::mulBoolTrueFalse() {
    buildSimple("Bool", "True", "*", "False", false);
}

void TestMaster::mulBoolFalseTrue() {
    buildSimple("Bool", "False", "*", "True", false);
}

void TestMaster::mulBoolFalseFalse() {
    buildSimple("Bool", "False", "*", "False", false);
}

// Multiplication - Character

void TestMaster::mulCharStrStr() {
    buildSimple("String", "\"string\"", "*", "\"string\"", false);
}

void TestMaster::mulCharStrChar() {
    buildSimple("String", "\"string\"", "*", "'c'", false);

}

void TestMaster::mulCharCharStr() {
    buildSimple("Char", "'c'", "*", "\"string\"", false);

}

void TestMaster::mulCharCharChar() {
    buildSimple("Char", "'c'", "*", "'c'", false);

}

// Multiplication - Mix

void TestMaster::mulMixFloatInt() {
    buildSimple("Float", "2.0", "*", "2", false);
}

void TestMaster::mulMixIntFloat() {
    buildSimple("Int", "2", "*", "2.0", false);
}

/*
 * Division
 */

// Division - Integer

void TestMaster::divIntPosPos() {
    buildSimple("Int", "2", "/", "2", true);
}

void TestMaster::divIntZeroZero() {
    buildSimple("Int", "0", "/", "0", true);
}

void TestMaster::divIntPosNeg() {
    buildSimple("Int", "2", "/", "-2", true);
}

void TestMaster::divIntNegPos() {
    buildSimple("Int", "-2", "/", "2", true);
}

void TestMaster::divIntNegNeg() {
    buildSimple("Int", "-2", "/", "-2", true);
}

// Division - Float

void TestMaster::divFloatPosPos() {
    buildSimple("Float", "2.0", "/", "2.0", true);
}

void TestMaster::divFloatZeroZero() {
    buildSimple("Float", "0.0", "/", "0.0", true);
}

void TestMaster::divFloatPosNeg() {
    buildSimple("Float", "2.0", "/", "-2.0", true);
}

void TestMaster::divFloatNegPos() {
    buildSimple("Float", "-2.0", "/", "2.0", true);
}

void TestMaster::divFloatNegNeg() {
    buildSimple("Float", "-2.0", "/", "-2.0", true);
}

// Division - Boolean

void TestMaster::divBoolTrueTrue() {
    buildSimple("Bool", "True", "/", "True", false);
}

void TestMaster::divBoolTrueFalse() {
    buildSimple("Bool", "True", "/", "False", false);
}

void TestMaster::divBoolFalseTrue() {
    buildSimple("Bool", "False", "/", "True", false);
}

void TestMaster::divBoolFalseFalse() {
    buildSimple("Bool", "False", "/", "False", false);
}

// Division - Character

void TestMaster::divCharStrStr() {
    buildSimple("String", "\"string\"", "/", "\"string\"", false);
}

void TestMaster::divCharStrChar() {
    buildSimple("String", "\"string\"", "/", "'c'", false);

}

void TestMaster::divCharCharStr() {
    buildSimple("Char", "'c'", "/", "\"string\"", false);

}

void TestMaster::divCharCharChar() {
    buildSimple("Char", "'c'", "/", "'c'", false);

}

// Division - Mix

void TestMaster::divMixFloatInt() {
    buildSimple("Float", "2.0", "/", "2", false);
}

void TestMaster::divMixIntFloat() {
    buildSimple("Int", "2", "/", "2.0", false);
}

/*
 * Modulo
 */

// Modulo - Integer

void TestMaster::modIntPosPos() {
    buildSimple("Int", "2", "%", "2", true);
}

void TestMaster::modIntZeroZero() {
    buildSimple("Int", "0", "%", "0", true);
}

void TestMaster::modIntPosNeg() {
    buildSimple("Int", "2", "%", "-2", true);
}

void TestMaster::modIntNegPos() {
    buildSimple("Int", "-2", "%", "2", true);
}

void TestMaster::modIntNegNeg() {
    buildSimple("Int", "-2", "%", "-2", true);
}

// Modulo - Float

void TestMaster::modFloatPosPos() {
    buildSimple("Float", "2.0", "%", "2.0", true);
}

void TestMaster::modFloatZeroZero() {
    buildSimple("Float", "0.0", "%", "0.0", true);
}

void TestMaster::modFloatPosNeg() {
    buildSimple("Float", "2.0", "%", "-2.0", true);
}

void TestMaster::modFloatNegPos() {
    buildSimple("Float", "-2.0", "%", "2.0", true);
}

void TestMaster::modFloatNegNeg() {
    buildSimple("Float", "-2.0", "%", "-2.0", true);
}

// Modulo - Boolean

void TestMaster::modBoolTrueTrue() {
    buildSimple("Bool", "True", "%", "True", false);
}

void TestMaster::modBoolTrueFalse() {
    buildSimple("Bool", "True", "%", "False", false);
}

void TestMaster::modBoolFalseTrue() {
    buildSimple("Bool", "False", "%", "True", false);
}

void TestMaster::modBoolFalseFalse() {
    buildSimple("Bool", "False", "%", "False", false);
}

// Modulo - Character

void TestMaster::modCharStrStr() {
    buildSimple("String", "\"string\"", "%", "\"string\"", false);
}

void TestMaster::modCharStrChar() {
    buildSimple("String", "\"string\"", "%", "'c'", false);

}

void TestMaster::modCharCharStr() {
    buildSimple("Char", "'c'", "%", "\"string\"", false);

}

void TestMaster::modCharCharChar() {
    buildSimple("Char", "'c'", "%", "'c'", false);

}

// Modulo - Mix

void TestMaster::modMixFloatInt() {
    buildSimple("Float", "2.0", "%", "2", false);
}

void TestMaster::modMixIntFloat() {
    buildSimple("Int", "2", "%", "2.0", false);
}

/*
 * Binary Operators
 */

// Binary - Equal

void TestMaster::binEqualTrueTrue() {
    buildSimple("Bool", "True", "==", "True", true);
}

void TestMaster::binEqualTrueFalse() {
    buildSimple("Bool", "True", "==", "False", true);
}

void TestMaster::binEqualFalseTrue() {
    buildSimple("Bool", "False", "==", "True", true);
}

void TestMaster::binEqualFalseFalse() {
    buildSimple("Bool", "False", "==", "False", true);
}

// Binary - NotEqual

void TestMaster::binNotEqualTrueTrue() {
    buildSimple("Bool", "True", "!=", "True", true);
}

void TestMaster::binNotEqualTrueFalse() {
    buildSimple("Bool", "True", "!=", "False", true);
}

void TestMaster::binNotEqualFalseTrue() {
    buildSimple("Bool", "False", "!=", "True", true);
}

void TestMaster::binNotEqualFalseFalse() {
    buildSimple("Bool", "False", "!=", "False", true);
}

// Binary - And

void TestMaster::binAndTrueTrue() {
    buildSimple("Bool", "True", "&&", "True", true);
}

void TestMaster::binAndTrueFalse() {
    buildSimple("Bool", "True", "&&", "False", true);
}

void TestMaster::binAndFalseTrue() {
    buildSimple("Bool", "False", "&&", "True", true);
}

void TestMaster::binAndFalseFalse() {
    buildSimple("Bool", "False", "&&", "False", true);
}

// Binary - Or

void TestMaster::binOrTrueTrue() {
    buildSimple("Bool", "True", "||", "True", true);
}

void TestMaster::binOrTrueFalse() {
    buildSimple("Bool", "True", "||", "False", true);
}

void TestMaster::binOrFalseTrue() {
    buildSimple("Bool", "False", "||", "True", true);
}

void TestMaster::binOrFalseFalse() {
    buildSimple("Bool", "False", "||", "False", true);
}

// Binary - GrEq

void TestMaster::binGrEqTrueTrue() {
    buildSimple("Bool", "True", ">=", "True", false);
}

void TestMaster::binGrEqTrueFalse() {
    buildSimple("Bool", "True", ">=", "False", false);
}

void TestMaster::binGrEqFalseTrue() {
    buildSimple("Bool", "False", ">=", "True", false);
}

void TestMaster::binGrEqFalseFalse() {
    buildSimple("Bool", "False", ">=", "False", false);
}

// Binary - LeEq

void TestMaster::binLeEqTrueTrue() {
    buildSimple("Bool", "True", "<=", "True", false);
}

void TestMaster::binLeEqTrueFalse() {
    buildSimple("Bool", "True", "<=", "False", false);
}

void TestMaster::binLeEqFalseTrue() {
    buildSimple("Bool", "False", "<=", "True", false);
}

void TestMaster::binLeEqFalseFalse() {
    buildSimple("Bool", "False", "<=", "False", false);
}

// Binary - Great

void TestMaster::binGreatTrueTrue() {
    buildSimple("Bool", "True", ">", "True", false);
}

void TestMaster::binGreatTrueFalse() {
    buildSimple("Bool", "True", ">", "False", false);
}

void TestMaster::binGreatFalseTrue() {
    buildSimple("Bool", "False", ">", "True", false);
}

void TestMaster::binGreatFalseFalse() {
    buildSimple("Bool", "False", ">", "False", false);
}

// Binary - Less

void TestMaster::binLessTrueTrue() {
    buildSimple("Bool", "True", "<", "True", false);
}

void TestMaster::binLessTrueFalse() {
    buildSimple("Bool", "True", "<", "False", false);
}

void TestMaster::binLessFalseTrue() {
    buildSimple("Bool", "False", "<", "True", false);
}

void TestMaster::binLessFalseFalse() {
    buildSimple("Bool", "False", "<", "False", false);
}
