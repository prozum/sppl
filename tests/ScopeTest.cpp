#include "Test.h"

void Test::scopeNormal() {
    bool compStatus = compileChecker("scope/scopeNormal.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "10");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::scopeSamePat() {
    bool compStatus = compileChecker("scope/scopeSamePat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::scopeSameFunc() {
    bool compStatus = compileChecker("scope/scopeSameFunc.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::scopeUndeclId() {
    bool compStatus = compileChecker("scope/scopeUndeclId.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}