#include "Test.h"


void Test::binEqualIntInt() { 
    bool compStatus = compileChecker("binEqualIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualIntInt() { 
    bool compStatus = compileChecker("binNotEqualIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndIntInt() { 
    bool compStatus = compileChecker("binAndIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrIntInt() { 
    bool compStatus = compileChecker("binOrIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqIntInt() { 
    bool compStatus = compileChecker("binGrEqIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqIntInt() { 
    bool compStatus = compileChecker("binLeEqIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatIntInt() { 
    bool compStatus = compileChecker("binGreatIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessIntInt() { 
    bool compStatus = compileChecker("binLessIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualFloatFloat() { 
    bool compStatus = compileChecker("binEqualFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualFloatFloat() { 
    bool compStatus = compileChecker("binNotEqualFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndFloatFloat() { 
    bool compStatus = compileChecker("binAndFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrFloatFloat() { 
    bool compStatus = compileChecker("binOrFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqFloatFloat() { 
    bool compStatus = compileChecker("binGrEqFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqFloatFloat() { 
    bool compStatus = compileChecker("binLeEqFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatFloatFloat() { 
    bool compStatus = compileChecker("binGreatFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessFloatFloat() { 
    bool compStatus = compileChecker("binLessFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

