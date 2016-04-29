#include "Test.h"
void Test::fileImpOne() {
    bool compStatus = compileChecker("file/fileImpOne.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::fileImpTwo() {
    bool compStatus = compileChecker("file/fileImpTwo.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("8");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::fileImpThree() {
    bool compStatus = compileChecker("file/fileImpThree.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("12");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::fileImpDup() {
    bool compStatus = compileChecker("file/fileImpDup.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::fileImpTwice() {
    bool compStatus = compileChecker("file/fileImpTwice.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::fileImpWrong() {
    bool compStatus = compileChecker("file/fileIncludeWrong.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::fileMainInOther() {
    bool compStatus = compileChecker("file/fileImpMainInOther.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::fileImpNested() {
    bool compStatus = compileChecker("file/fileImpNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::fileImpNestedDup() {
    bool compStatus = compileChecker("file/fileImpNestedDup.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}