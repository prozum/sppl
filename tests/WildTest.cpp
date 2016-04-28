#include "Test.h"

void Test::wildInt() {
    bool compStatus = compileChecker("wild/wildInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildFloat() {
    bool compStatus = compileChecker("wild/wildFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2.0");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildBool() {
    bool compStatus = compileChecker("wild/wildBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildChar() {
    bool compStatus = compileChecker("wild/wildChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("'c'");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildString() {
    bool compStatus = compileChecker("wild/wildString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("\"string\"");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildDouble() {
    bool compStatus = compileChecker("wild/wildDouble.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildTwoIntFirst() {
    bool compStatus = compileChecker("wild/wildTwoIntFirst.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildTwoIntSecond() {
    bool compStatus = compileChecker("wild/wildTwoIntSecond.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildMain() {
    bool compStatus = compileChecker("wild/wildMain.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::wildWrong() {
    bool compStatus = compileChecker("wild/wildWrong.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}