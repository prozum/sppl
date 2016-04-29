#include "Test.h"

void Test::conIntList() {
    bool compStatus = compileChecker("con/conIntList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1, 2, 3, 4]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conFloatList() {
    bool compStatus = compileChecker("con/conFloatList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1.000000, 2.000000, 3.000000, 4.000000]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conBoolList() {
    bool compStatus = compileChecker("con/conBoolList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[True, True, False, False]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conCharList() {
    bool compStatus = compileChecker("con/conCharList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("['c', 'h', 'a', 'r']");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conStringList() {
    bool compStatus = compileChecker("con/conStringList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[\"string\", \"string\", \"string\", \"string\"]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conNestedList() {
    bool compStatus = compileChecker("con/conNestedList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[[1, 2], [3, 4], [5, 6], [7, 8]]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conTupleList() {
    bool compStatus = compileChecker("con/conTupleList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[(1, True), (2, False), (3, True), (4, False)]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conTuple() {
    bool compStatus = compileChecker("con/conTuple.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conInt() {
    bool compStatus = compileChecker("con/conInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);

}

void Test::conFloat() {
    bool compStatus = compileChecker("con/conFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conBool() {
    bool compStatus = compileChecker("con/conBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conChar() {
    bool compStatus = compileChecker("con/conChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conString() {
    bool compStatus = compileChecker("con/conString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("\"concatstring\"");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conMix() {
    bool compStatus = compileChecker("con/conMix.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conFuncResOne() {
    bool compStatus = compileChecker("con/conFunResOne.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1, 2, 3, 4]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conFuncResTwo() {
    bool compStatus = compileChecker("con/conFunResTwo.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1, 2, 3, 4]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conEmpty() {
    bool compStatus = compileChecker("con/conEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}