#include "Test.h"

void Test::sigLengthZeroInt() { 
    bool compStatus = compileChecker("sig/sigLengthZeroInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthOneInt() { 
    bool compStatus = compileChecker("sig/sigLengthOneInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthTwoInt() { 
    bool compStatus = compileChecker("sig/sigLengthTwoInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthThreeInt() { 
    bool compStatus = compileChecker("sig/sigLengthThreeInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthFourInt() { 
    bool compStatus = compileChecker("sig/sigLengthFourInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthFiveInt() { 
    bool compStatus = compileChecker("sig/sigLengthFiveInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthSixInt() { 
    bool compStatus = compileChecker("sig/sigLengthSixInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthSevenInt() { 
    bool compStatus = compileChecker("sig/sigLengthSevenInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthEightInt() { 
    bool compStatus = compileChecker("sig/sigLengthEightInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthNineInt() { 
    bool compStatus = compileChecker("sig/sigLengthNineInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::sigLengthTenInt() { 
    bool compStatus = compileChecker("sig/sigLengthTenInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::patLengthSameInt() { 
    bool compStatus = compileChecker("patLengthSameInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "2");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::patLengthLessInt() { 
    bool compStatus = compileChecker("patLengthLessInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::patLengthMoreInt() { 
    bool compStatus = compileChecker("patLengthMoreInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}