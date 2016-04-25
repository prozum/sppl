#include "Test.h"

// These tests are based on the tests used in GNU GCC
// gcc/gcc/testsuite/gcc.dg/cpp/if-oppr.c

void Test::precAndOr() {
    bool compStatus = compileChecker("prec/precAndOr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::precEqualAnd() {
    bool compStatus = compileChecker("prec/precEqualAnd.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::precLessEqual() {
    bool compStatus = compileChecker("prec/precLessEqual.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::precAddLess() {
    bool compStatus = compileChecker("prec/precAddLess.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("False");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::precMulAdd() {
    bool compStatus = compileChecker("prec/precMulAdd.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("14");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::precNegMul() {
    bool compStatus = compileChecker("prec/precAndOr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("-4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}