#include "Test.h"

void Test::subIntPosPos() { 
    bool compStatus = compileChecker("sub/subIntPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("0");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subIntZeroZero() { 
    bool compStatus = compileChecker("sub/subIntZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("0");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subIntPosNeg() { 
    bool compStatus = compileChecker("sub/subIntPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subIntNegPos() { 
    bool compStatus = compileChecker("sub/subIntNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("-4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subIntNegNeg() { 
    bool compStatus = compileChecker("sub/subIntNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("0");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subFloatPosPos() { 
    bool compStatus = compileChecker("sub/subFloatPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("0.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subFloatZeroZero() { 
    bool compStatus = compileChecker("sub/subFloatZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("0.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subFloatPosNeg() { 
    bool compStatus = compileChecker("sub/subFloatPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("4.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subFloatNegPos() { 
    bool compStatus = compileChecker("sub/subFloatNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("-4.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subFloatNegNeg() { 
    bool compStatus = compileChecker("sub/subFloatNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("0.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::subBoolTrueTrue() { 
    bool compStatus = compileChecker("sub/subBoolTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subBoolTrueFalse() { 
    bool compStatus = compileChecker("sub/subBoolTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subBoolFalseTrue() { 
    bool compStatus = compileChecker("sub/subBoolFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subBoolFalseFalse() { 
    bool compStatus = compileChecker("sub/subBoolFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subCharStrStr() { 
    bool compStatus = compileChecker("sub/subCharStrStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subCharStrCha() { 
    bool compStatus = compileChecker("sub/subCharStrCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subCharChaStr() { 
    bool compStatus = compileChecker("sub/subCharChaStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subCharChaCha() { 
    bool compStatus = compileChecker("sub/subCharChaCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subMixFloatInt() { 
    bool compStatus = compileChecker("sub/subMixFloatInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::subMixIntFloat() { 
    bool compStatus = compileChecker("sub/subMixIntFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}