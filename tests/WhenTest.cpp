#include "Test.h"

void Test::whenTrue() {
    bool compStatus = compileChecker("when/whenTrue.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenFalse() {
    bool compStatus = compileChecker("when/whenFalse.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenAnd() {
    bool compStatus = compileChecker("when/whenAnd.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenOr() {
    bool compStatus = compileChecker("when/whenOr.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenEqual() {
    bool compStatus = compileChecker("when/whenEqual.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenNotEqual() {
    bool compStatus = compileChecker("when/whenNotEqual.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenLessEqual() {
    bool compStatus = compileChecker("when/whenLessEqual.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenGreatEqual() {
    bool compStatus = compileChecker("when/whenGreatEqual.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenLess() {
    bool compStatus = compileChecker("when/whenLess.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenGreat() {
    bool compStatus = compileChecker("when/whenGreat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenMultiple() {
    bool compStatus = compileChecker("when/whenMultiple.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenNotScope() {
    bool compStatus = compileChecker("when/whenNotScope.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}

void Test::whenEmpty() {
    bool compStatus = compileChecker("when/whenEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}

void Test::whenWhen() {
    bool compStatus = compileChecker("when/whenWhen.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}

void Test::whenFuncBool() {
    bool compStatus = compileChecker("when/whenFuncBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("True");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::whenFuncNotBool() {
    bool compStatus = compileChecker("when/whenFuncNotBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, !compStatus);
}