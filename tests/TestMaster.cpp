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