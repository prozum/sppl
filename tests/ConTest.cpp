#include "Test.h"

void Test::conIntList() {
    bool compStatus = compileChecker("concat/conIntList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1, 2, 3, 4]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conFloatList() {
    bool compStatus = compileChecker("concat/conFloatList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1.000000, 2.000000, 3.000000, 4.000000]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conBoolList() {
    bool compStatus = compileChecker("concat/conBoolList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[True, True, False, False]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conCharList() {
    bool compStatus = compileChecker("concat/conCharList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("['c', 'h', 'a', 'r']");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conStringList() {
    bool compStatus = compileChecker("concat/conStringList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[\"string\", \"string\", \"string\", \"string\"]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conNestedList() {
    bool compStatus = compileChecker("concat/conNestedList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[[1, 2], [3, 4], [5, 6], [7, 8]]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conTupleList() {
    bool compStatus = compileChecker("concat/conTupleList.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[(1, True), (2, False), (3, True), (4, False)]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conTuple() {
    bool compStatus = compileChecker("concat/conTuple.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conInt() {
    bool compStatus = compileChecker("concat/conInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);

}

void Test::conFloat() {
    bool compStatus = compileChecker("concat/conFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conBool() {
    bool compStatus = compileChecker("concat/conBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conChar() {
    bool compStatus = compileChecker("concat/conChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conString() {
    bool compStatus = compileChecker("concat/conString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("\"concatstring\"");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conMix() {
    bool compStatus = compileChecker("concat/conMix.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::conFuncResOne() {
    bool compStatus = compileChecker("concat/conFuncResOne.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1, 2, 3, 4]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conFuncResTwo() {
    bool compStatus = compileChecker("concat/conFuncResTwo.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("[1, 2, 3, 4]");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::conEmpty() {
    bool compStatus = compileChecker("concat/conEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}