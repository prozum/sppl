#include "Test.h"

void Test::dotNormal() {
    bool compStatus = compileChecker("dot/dotNormal.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("2.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::dotNumDot() {
    bool compStatus = compileChecker("dot/dotNumDot.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}

void Test::dotDotNum() {
    bool compStatus = compileChecker("dot/dotDotNum.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}