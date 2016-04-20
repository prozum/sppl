#include "Test.h"

void Test::divIntPosPos() { 
    bool compStatus = compileChecker("div/divIntPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "1");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divIntZeroZero() { 
    bool compStatus = compileChecker("div/divIntZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "0");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divIntPosNeg() { 
    bool compStatus = compileChecker("div/divIntPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-1");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divIntNegPos() { 
    bool compStatus = compileChecker("div/divIntNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-1");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divIntNegNeg() { 
    bool compStatus = compileChecker("div/divIntNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "1");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divFloatPosPos() { 
    bool compStatus = compileChecker("div/divFloatPosPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "1.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divFloatZeroZero() { 
    bool compStatus = compileChecker("div/divFloatZeroZero.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "0.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divFloatPosNeg() { 
    bool compStatus = compileChecker("div/divFloatPosNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-1.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divFloatNegPos() { 
    bool compStatus = compileChecker("div/divFloatNegPos.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "-1.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divFloatNegNeg() { 
    bool compStatus = compileChecker("div/divFloatNegNeg.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "1.000000");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::divBoolTrueTrue() { 
    bool compStatus = compileChecker("div/divBoolTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divBoolTrueFalse() { 
    bool compStatus = compileChecker("div/divBoolTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divBoolFalseTrue() { 
    bool compStatus = compileChecker("div/divBoolFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divBoolFalseFalse() { 
    bool compStatus = compileChecker("div/divBoolFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divCharStrStr() { 
    bool compStatus = compileChecker("div/divCharStrStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);

}

void Test::divCharStrCha() { 
    bool compStatus = compileChecker("div/divCharStrCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divCharChaStr() { 
    bool compStatus = compileChecker("div/divCharChaStr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divCharChaCha() { 
    bool compStatus = compileChecker("div/divCharChaCha.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divMixFloatInt() { 
    bool compStatus = compileChecker("div/divMixFloatInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::divMixIntFloat() { 
    bool compStatus = compileChecker("div/divMixIntFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

