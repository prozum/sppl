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
    // Addition - Boolean
    CPPUNIT_TEST(addBoolTrueTrue);
    CPPUNIT_TEST(addBoolTrueFalse);
    CPPUNIT_TEST(addBoolFalseTrue);
    CPPUNIT_TEST(addBoolFalseFalse);
    // Addition - Character
    CPPUNIT_TEST(addCharStrStr);
    CPPUNIT_TEST(addCharStrChar);
    CPPUNIT_TEST(addCharCharStr);
    CPPUNIT_TEST(addCharCharChar);
    // Addition - Mix
    CPPUNIT_TEST(addMixFloatInt);
    CPPUNIT_TEST(addMixIntFloat);

    // Subtraction - Integer
    CPPUNIT_TEST(subIntPosPos);
    CPPUNIT_TEST(subIntZeroZero);
    CPPUNIT_TEST(subIntNegPos);
    CPPUNIT_TEST(subIntPosNeg);
    CPPUNIT_TEST(subIntNegNeg);
    // Subtraction - Float
    CPPUNIT_TEST(subFloatPosPos);
    CPPUNIT_TEST(subFloatZeroZero);
    CPPUNIT_TEST(subFloatPosNeg);
    CPPUNIT_TEST(subFloatNegPos);
    CPPUNIT_TEST(subFloatNegNeg);
    // Subtraction - Boolean
    CPPUNIT_TEST(subBoolTrueTrue);
    CPPUNIT_TEST(subBoolTrueFalse);
    CPPUNIT_TEST(subBoolFalseTrue);
    CPPUNIT_TEST(subBoolFalseFalse);
    // Subtraction - Character
    CPPUNIT_TEST(subCharStrStr);
    CPPUNIT_TEST(subCharStrChar);
    CPPUNIT_TEST(subCharCharStr);
    CPPUNIT_TEST(subCharCharChar);
    // Subtraction - Mix
    CPPUNIT_TEST(subMixFloatInt);
    CPPUNIT_TEST(subMixIntFloat);

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
    // Addition - Integer
    void addIntPosPos();
    void addIntZeroZero();
    void addIntPosNeg();
    void addIntNegPos();
    void addIntNegNeg();

    // Addition - Float
    void addFloatPosPos();
    void addFloatZeroZero();
    void addFloatPosNeg();
    void addFloatNegPos();
    void addFloatNegNeg();

    // Addition - Boolean
    void addBoolTrueTrue();
    void addBoolTrueFalse();
    void addBoolFalseTrue();
    void addBoolFalseFalse();

    // Addition - Character
    void addCharStrStr();
    void addCharStrChar();
    void addCharCharStr();
    void addCharCharChar();

    // Addition - Mix
    void addMixIntFloat();
    void addMixFloatInt();

    // Subtraction - Integer
    void subIntPosPos();
    void subIntZeroZero();
    void subIntPosNeg();
    void subIntNegPos();
    void subIntNegNeg();

    // Subtraction - Float
    void subFloatPosPos();
    void subFloatZeroZero();
    void subFloatPosNeg();
    void subFloatNegPos();
    void subFloatNegNeg();

    // Subtraction - Boolean
    void subBoolTrueTrue();
    void subBoolTrueFalse();
    void subBoolFalseTrue();
    void subBoolFalseFalse();

    // Subtraction - Character
    void subCharStrStr();
    void subCharStrChar();
    void subCharCharStr();
    void subCharCharChar();

    // Subtraction - Mix
    void subMixIntFloat();
    void subMixFloatInt();
    
    void compileChecker(bool success);
    void buildSimple(std::string pattern, std::string left, std::string op, std::string right, bool status);
};