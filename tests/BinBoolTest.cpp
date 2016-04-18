#include "Test.h"


void Test::binEqualTrueTrue() { 
    bool compStatus = compileChecker("binEqualTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualTrueFalse() { 
    bool compStatus = compileChecker("binEqualTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualFalseTrue() { 
    bool compStatus = compileChecker("binEqualFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualFalseFalse() { 
    bool compStatus = compileChecker("binEqualFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualTrueTrue() { 
    bool compStatus = compileChecker("binNotEqualTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualTrueFalse() { 
    bool compStatus = compileChecker("binNotEqualTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualFalseTrue() { 
    bool compStatus = compileChecker("binNotEqualFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualFalseFalse() { 
    bool compStatus = compileChecker("binNotEqualFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndTrueTrue() { 
    bool compStatus = compileChecker("binAndTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndTrueFalse() { 
    bool compStatus = compileChecker("binAndTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndFalseTrue() { 
    bool compStatus = compileChecker("binAndFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndFalseFalse() { 
    bool compStatus = compileChecker("binAndFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrTrueTrue() { 
    bool compStatus = compileChecker("binOrTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrTrueFalse() { 
    bool compStatus = compileChecker("binOrTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrFalseTrue() { 
    bool compStatus = compileChecker("binOrFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrFalseFalse() { 
    bool compStatus = compileChecker("binOrFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqTrueTrue() { 
    bool compStatus = compileChecker("binGrEqTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqTrueFalse() { 
    bool compStatus = compileChecker("binGrEqTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqFalseTrue() { 
    bool compStatus = compileChecker("binGrEqFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqFalseFalse() { 
    bool compStatus = compileChecker("binGrEqFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqTrueTrue() { 
    bool compStatus = compileChecker("binLeEqTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqTrueFalse() { 
    bool compStatus = compileChecker("binLeEqTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqFalseTrue() { 
    bool compStatus = compileChecker("binLeEqFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqFalseFalse() { 
    bool compStatus = compileChecker("binLeEqFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatTrueTrue() { 
    bool compStatus = compileChecker("binGreatTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatTrueFalse() { 
    bool compStatus = compileChecker("binGreatTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatFalseTrue() { 
    bool compStatus = compileChecker("binGreatFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatFalseFalse() { 
    bool compStatus = compileChecker("binGreatFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessTrueTrue() { 
    bool compStatus = compileChecker("binLessTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessTrueFalse() { 
    bool compStatus = compileChecker("binLessTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessFalseTrue() { 
    bool compStatus = compileChecker("binLessFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessFalseFalse() { 
    bool compStatus = compileChecker("binLessFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

