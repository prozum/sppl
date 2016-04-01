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

// Addition - Integer

void TestMaster::addIntPosPos() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = 2 + 2";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addIntZeroZero() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = 0 + 0";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addIntPosNeg() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = 2 + -2";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addIntNegPos() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = -2 + 2";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addIntNegNeg() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = -2 + -2";
    sourceFile.close();
    compileChecker(true);
}

// Addition - Float

void TestMaster::addFloatPosPos() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Float | = 2.0 + 2.0";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addFloatZeroZero() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Float | = 0.0 + 0.0";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addFloatPosNeg() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Float | = 2.0 + -2.0";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addFloatNegPos() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Float | = -2.0 + 2.0";
    sourceFile.close();
    compileChecker(true);
}

void TestMaster::addFloatNegNeg() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Float | = -2.0 + -2.0";
    sourceFile.close();
    compileChecker(true);
}

