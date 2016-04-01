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
    CPPUNIT_TEST(addPosPos);
    CPPUNIT_TEST(addZeroZero);
    CPPUNIT_TEST(addNegPos);
    CPPUNIT_TEST(addPosNeg);
    CPPUNIT_TEST(addNegNeg);
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
    void addPosPos();
    void addZeroZero();
    void addPosNeg();
    void addNegPos();
    void addNegNeg();

    void internal(bool success);
};