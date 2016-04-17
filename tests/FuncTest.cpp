#include "Test.h"


void Test::funcRetInt() { 
    bool compStatus = compileChecker("funcRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetFloat() { 
    bool compStatus = compileChecker("funcRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetBool() { 
    bool compStatus = compileChecker("funcRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetChar() { 
    bool compStatus = compileChecker("funcRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetString() { 
    bool compStatus = compileChecker("funcRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntRetInt() { 
    bool compStatus = compileChecker("funcIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcFloatRetFloat() { 
    bool compStatus = compileChecker("funcFloatRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcBoolRetBool() { 
    bool compStatus = compileChecker("funcBoolRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcCharRetChar() { 
    bool compStatus = compileChecker("funcCharRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcStringRetString() { 
    bool compStatus = compileChecker("funcStringRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntIntRetInt() { 
    bool compStatus = compileChecker("funcIntIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcFloatFloatRetFloat() { 
    bool compStatus = compileChecker("funcFloatFloatRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcBoolBoolRetBool() { 
    bool compStatus = compileChecker("funcBoolBoolRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcCharCharRetChar() { 
    bool compStatus = compileChecker("funcCharCharRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcStringStringRetString() { 
    bool compStatus = compileChecker("funcStringStringRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListIntRetListInt() { 
    bool compStatus = compileChecker("funcListIntRetListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListFloatRetListFloat() { 
    bool compStatus = compileChecker("funcListFloatRetListFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1.000000]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListBoolRetListBool() { 
    bool compStatus = compileChecker("funcListBoolRetListBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[True]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListCharRetListChar() { 
    bool compStatus = compileChecker("funcListCharRetListChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "['c']");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListStringRetListString() { 
    bool compStatus = compileChecker("funcListStringRetListString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[\"string\"]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleIntIntRetTupleIntInt() { 
    bool compStatus = compileChecker("funcTupleIntIntRetTupleIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2, 2)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleFloatFloatRetTupleFloatFloat() { 
    bool compStatus = compileChecker("funcTupleFloatFloatRetTupleFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2.000000, 2.000000)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleBoolBoolRetTupleBoolBool() { 
    bool compStatus = compileChecker("funcTupleBoolBoolRetTupleBoolBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(True, False)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleCharCharRetTupleCharChar() { 
    bool compStatus = compileChecker("funcTupleCharCharRetTupleCharChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "('c', 'h')");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleStringStringRetTupleStringString() { 
    bool compStatus = compileChecker("funcTupleStringStringRetTupleStringString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(\"string\", \"string\")");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntToIntRetInt() { 
    bool compStatus = compileChecker("funcIntToIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcFloatToFloatRetFloat() { 
    bool compStatus = compileChecker("funcFloatToFloatRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcBoolToBoolRetBool() { 
    bool compStatus = compileChecker("funcBoolToBoolRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcCharToCharRetChar() { 
    bool compStatus = compileChecker("funcCharToCharRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcStringToStringRetString() { 
    bool compStatus = compileChecker("funcStringToStringRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListIntToListIntRetListInt() { 
    bool compStatus = compileChecker("funcListIntToListIntRetListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleIntIntToTupleIntIntRetInt() { 
    bool compStatus = compileChecker("funcTupleIntIntToTupleIntIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(1, 2)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntToIntRetToIntRetToInt() { 
    bool compStatus = compileChecker("funcIntToIntRetToIntRetToInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

