#include "Test.h"


void Test::binEqualTrueTrue() { 
    bool compStatus = compileChecker("bin/binEqualTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualTrueFalse() { 
    bool compStatus = compileChecker("bin/binEqualTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualFalseTrue() { 
    bool compStatus = compileChecker("bin/binEqualFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualFalseFalse() { 
    bool compStatus = compileChecker("bin/binEqualFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualTrueTrue() { 
    bool compStatus = compileChecker("bin/binNotEqualTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualTrueFalse() { 
    bool compStatus = compileChecker("bin/binNotEqualTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualFalseTrue() { 
    bool compStatus = compileChecker("bin/binNotEqualFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualFalseFalse() { 
    bool compStatus = compileChecker("bin/binNotEqualFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndTrueTrue() { 
    bool compStatus = compileChecker("bin/binAndTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndTrueFalse() { 
    bool compStatus = compileChecker("bin/binAndTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndFalseTrue() { 
    bool compStatus = compileChecker("bin/binAndFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndFalseFalse() { 
    bool compStatus = compileChecker("bin/binAndFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrTrueTrue() { 
    bool compStatus = compileChecker("bin/binOrTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrTrueFalse() { 
    bool compStatus = compileChecker("bin/binOrTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrFalseTrue() { 
    bool compStatus = compileChecker("bin/binOrFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrFalseFalse() { 
    bool compStatus = compileChecker("bin/binOrFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqTrueTrue() { 
    bool compStatus = compileChecker("bin/binGrEqTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqTrueFalse() { 
    bool compStatus = compileChecker("bin/binGrEqTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqFalseTrue() { 
    bool compStatus = compileChecker("bin/binGrEqFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqFalseFalse() { 
    bool compStatus = compileChecker("bin/binGrEqFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqTrueTrue() { 
    bool compStatus = compileChecker("bin/binLeEqTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqTrueFalse() { 
    bool compStatus = compileChecker("bin/binLeEqTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqFalseTrue() { 
    bool compStatus = compileChecker("bin/binLeEqFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqFalseFalse() { 
    bool compStatus = compileChecker("bin/binLeEqFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatTrueTrue() { 
    bool compStatus = compileChecker("bin/binGreatTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatTrueFalse() { 
    bool compStatus = compileChecker("bin/binGreatTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatFalseTrue() { 
    bool compStatus = compileChecker("bin/binGreatFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatFalseFalse() { 
    bool compStatus = compileChecker("bin/binGreatFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessTrueTrue() { 
    bool compStatus = compileChecker("bin/binLessTrueTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessTrueFalse() { 
    bool compStatus = compileChecker("bin/binLessTrueFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessFalseTrue() { 
    bool compStatus = compileChecker("bin/binLessFalseTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessFalseFalse() { 
    bool compStatus = compileChecker("bin/binLessFalseFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

// VALUES

void Test::binEqualIntInt() {
    bool compStatus = compileChecker("bin/binEqualIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualIntInt() {
    bool compStatus = compileChecker("bin/binNotEqualIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndIntInt() {
    bool compStatus = compileChecker("bin/binAndIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrIntInt() {
    bool compStatus = compileChecker("bin/binOrIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqIntInt() {
    bool compStatus = compileChecker("bin/binGrEqIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqIntInt() {
    bool compStatus = compileChecker("bin/binLeEqIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatIntInt() {
    bool compStatus = compileChecker("bin/binGreatIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessIntInt() {
    bool compStatus = compileChecker("bin/binLessIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binEqualFloatFloat() {
    bool compStatus = compileChecker("bin/binEqualFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binNotEqualFloatFloat() {
    bool compStatus = compileChecker("bin/binNotEqualFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binAndFloatFloat() {
    bool compStatus = compileChecker("bin/binAndFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binOrFloatFloat() {
    bool compStatus = compileChecker("bin/binOrFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGrEqFloatFloat() {
    bool compStatus = compileChecker("bin/binGrEqFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLeEqFloatFloat() {
    bool compStatus = compileChecker("bin/binLeEqFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binGreatFloatFloat() {
    bool compStatus = compileChecker("bin/binGreatFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::binLessFloatFloat() {
    bool compStatus = compileChecker("bin/binLessFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}