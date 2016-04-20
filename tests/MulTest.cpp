#include "Test.h"

void Test::mulIntPosPos() { 
    bool compStatus = compileChecker("mul/mulIntPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulIntZeroZero() { 
    bool compStatus = compileChecker("mul/mulIntZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "0");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulIntPosNeg() { 
    bool compStatus = compileChecker("mul/mulIntPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulIntNegPos() { 
    bool compStatus = compileChecker("mul/mulIntNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulIntNegNeg() { 
    bool compStatus = compileChecker("mul/mulIntNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "4");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulFloatPosPos() { 
    bool compStatus = compileChecker("mul/mulFloatPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "4.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulFloatZeroZero() { 
    bool compStatus = compileChecker("mul/mulFloatZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "0.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulFloatPosNeg() { 
    bool compStatus = compileChecker("mul/mulFloatPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-4.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulFloatNegPos() { 
    bool compStatus = compileChecker("mul/mulFloatNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-4.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulFloatNegNeg() { 
    bool compStatus = compileChecker("mul/mulFloatNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "4.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::mulBoolTrueTrue() { 
    bool compStatus = compileChecker("mul/mulBoolTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulBoolTrueFalse() { 
    bool compStatus = compileChecker("mul/mulBoolTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulBoolFalseTrue() { 
    bool compStatus = compileChecker("mul/mulBoolFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulBoolFalseFalse() { 
    bool compStatus = compileChecker("mul/mulBoolFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulCharStrStr() { 
    bool compStatus = compileChecker("mul/mulCharStrStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulCharStrCha() { 
    bool compStatus = compileChecker("mul/mulCharStrCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulCharChaStr() { 
    bool compStatus = compileChecker("mul/mulCharChaStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulCharChaCha() { 
    bool compStatus = compileChecker("mul/mulCharChaCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulMixFloatInt() { 
    bool compStatus = compileChecker("mul/mulMixFloatInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::mulMixIntFloat() { 
    bool compStatus = compileChecker("mul/mulMixIntFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}