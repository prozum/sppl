#include "Test.h"


void Test::addIntPosPos() { 
    bool compStatus = compileChecker("addIntPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntZeroZero() { 
    bool compStatus = compileChecker("addIntZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntPosNeg() { 
    bool compStatus = compileChecker("addIntPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntNegPos() { 
    bool compStatus = compileChecker("addIntNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addIntNegNeg() { 
    bool compStatus = compileChecker("addIntNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatPosPos() { 
    bool compStatus = compileChecker("addFloatPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatZeroZero() { 
    bool compStatus = compileChecker("addFloatZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatPosNeg() { 
    bool compStatus = compileChecker("addFloatPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatNegPos() { 
    bool compStatus = compileChecker("addFloatNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addFloatNegNeg() { 
    bool compStatus = compileChecker("addFloatNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolTrueTrue() { 
    bool compStatus = compileChecker("addBoolTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolTrueFalse() { 
    bool compStatus = compileChecker("addBoolTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolFalseTrue() { 
    bool compStatus = compileChecker("addBoolFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addBoolFalseFalse() { 
    bool compStatus = compileChecker("addBoolFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharStrStr() { 
    bool compStatus = compileChecker("addCharStrStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharStrCha() { 
    bool compStatus = compileChecker("addCharStrCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharChaStr() { 
    bool compStatus = compileChecker("addCharChaStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addCharChaCha() { 
    bool compStatus = compileChecker("addCharChaCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addMixFloatInt() { 
    bool compStatus = compileChecker("addMixFloatInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::addMixIntFloat() { 
    bool compStatus = compileChecker("addMixIntFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

