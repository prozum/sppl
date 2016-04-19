#include "Test.h"


void Test::addIntPosPos() { 
    bool compStatus = compileChecker("add/addIntPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntZeroZero() { 
    bool compStatus = compileChecker("add/addIntZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntPosNeg() { 
    bool compStatus = compileChecker("add/addIntPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntNegPos() { 
    bool compStatus = compileChecker("add/addIntNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntNegNeg() { 
    bool compStatus = compileChecker("add/addIntNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatPosPos() { 
    bool compStatus = compileChecker("add/addFloatPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatZeroZero() { 
    bool compStatus = compileChecker("add/addFloatZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatPosNeg() { 
    bool compStatus = compileChecker("add/addFloatPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatNegPos() { 
    bool compStatus = compileChecker("add/addFloatNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatNegNeg() { 
    bool compStatus = compileChecker("add/addFloatNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolTrueTrue() { 
    bool compStatus = compileChecker("add/addBoolTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolTrueFalse() { 
    bool compStatus = compileChecker("add/addBoolTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolFalseTrue() { 
    bool compStatus = compileChecker("add/addBoolFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolFalseFalse() { 
    bool compStatus = compileChecker("add/addBoolFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharStrStr() { 
    bool compStatus = compileChecker("add/addCharStrStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharStrCha() { 
    bool compStatus = compileChecker("add/addCharStrCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharChaStr() { 
    bool compStatus = compileChecker("add/addCharChaStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharChaCha() { 
    bool compStatus = compileChecker("add/addCharChaCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addMixFloatInt() { 
    bool compStatus = compileChecker("add/addMixFloatInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addMixIntFloat() { 
    bool compStatus = compileChecker("add/addMixIntFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

