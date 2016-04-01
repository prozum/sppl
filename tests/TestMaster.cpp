#include "TestMaster.h"

void TestMaster::setUp() {

}

void TestMaster::tearDown() {
    remove("source.sppl");
    remove("target.body");
    remove("target.head");
    status = 0;
}

void TestMaster::internal(bool success) {
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
}

void TestMaster::addPosPos() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = 2 + 2";
    sourceFile.close();
    internal(true);
}

void TestMaster::addZeroZero() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = 0 + 0";
    sourceFile.close();
    internal(true);
}

void TestMaster::addPosNeg() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = 2 + -2";
    sourceFile.close();
    internal(true);
}

void TestMaster::addNegPos() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = -2 + 2";
    sourceFile.close();
    internal(true);
}

void TestMaster::addNegNeg() {
    ofstream sourceFile("source.sppl");
    sourceFile << "def main : Int | = -2 + -2";
    sourceFile.close();
    internal(true);
}

