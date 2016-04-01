#pragma once

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include "Compiler.h"
#include <fstream>
#include <cstring>

class TestMaster : public CppUnit::TestFixture{
    CPPUNIT_TEST_SUITE(TestMaster);
    // Addition - Integer
    CPPUNIT_TEST(addIntPosPos);
    CPPUNIT_TEST(addIntZeroZero);
    CPPUNIT_TEST(addIntNegPos);
    CPPUNIT_TEST(addIntPosNeg);
    CPPUNIT_TEST(addIntNegNeg);
    // Addition - Float
    CPPUNIT_TEST(addFloatPosPos);
    CPPUNIT_TEST(addFloatZeroZero);
    CPPUNIT_TEST(addFloatPosNeg);
    CPPUNIT_TEST(addFloatNegPos);
    CPPUNIT_TEST(addFloatNegNeg);
    CPPUNIT_TEST_SUITE_END();
public:
    ifstream in;
    ofstream out;
    ofstream hout;
    compiler::Backend backend = compiler::Backend::CPP ;
    int status = 0;

    void setUp();
    void tearDown();
protected:
    // Addition - Integers
    void addIntPosPos();
    void addIntZeroZero();
    void addIntPosNeg();
    void addIntNegPos();
    void addIntNegNeg();

    // Addition - Floats
    void addFloatPosPos();
    void addFloatZeroZero();
    void addFloatPosNeg();
    void addFloatNegPos();
    void addFloatNegNeg();

    void compileChecker(bool success);
};