#include "Test.h"


void Test::keywordInt() { 
    bool compStatus = compileChecker("keyword/keywordInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::keywordFloat() { 
    bool compStatus = compileChecker("keyword/keywordFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::keywordBool() { 
    bool compStatus = compileChecker("keyword/keywordBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::keywordChar() { 
    bool compStatus = compileChecker("keyword/keywordChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::keywordString() { 
    bool compStatus = compileChecker("keyword/keywordString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::keywordDef() { 
    bool compStatus = compileChecker("keyword/keywordDef.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::keywordMain() { 
    bool compStatus = compileChecker("keyword/keywordMain.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::comment() { 
    bool compStatus = compileChecker("comment.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

