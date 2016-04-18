#include "Test.h"


void Test::funcRetInt() { 
    bool compStatus = compileChecker("func/funcRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetFloat() { 
    bool compStatus = compileChecker("func/funcRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetBool() { 
    bool compStatus = compileChecker("func/funcRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetChar() { 
    bool compStatus = compileChecker("func/funcRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcRetString() { 
    bool compStatus = compileChecker("func/funcRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntRetInt() { 
    bool compStatus = compileChecker("func/funcIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcFloatRetFloat() { 
    bool compStatus = compileChecker("func/funcFloatRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcBoolRetBool() { 
    bool compStatus = compileChecker("func/funcBoolRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcCharRetChar() { 
    bool compStatus = compileChecker("func/funcCharRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcStringRetString() { 
    bool compStatus = compileChecker("func/funcStringRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntIntRetInt() { 
    bool compStatus = compileChecker("func/funcIntIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcFloatFloatRetFloat() { 
    bool compStatus = compileChecker("func/funcFloatFloatRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcBoolBoolRetBool() { 
    bool compStatus = compileChecker("func/funcBoolBoolRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcCharCharRetChar() { 
    bool compStatus = compileChecker("func/funcCharCharRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcStringStringRetString() { 
    bool compStatus = compileChecker("func/funcStringStringRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListIntRetListInt() { 
    bool compStatus = compileChecker("func/funcListIntRetListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListFloatRetListFloat() { 
    bool compStatus = compileChecker("func/funcListFloatRetListFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1.000000]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListBoolRetListBool() { 
    bool compStatus = compileChecker("func/funcListBoolRetListBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[True]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListCharRetListChar() { 
    bool compStatus = compileChecker("func/funcListCharRetListChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "['c']");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListStringRetListString() { 
    bool compStatus = compileChecker("func/funcListStringRetListString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[\"string\"]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleIntIntRetTupleIntInt() { 
    bool compStatus = compileChecker("func/funcTupleIntIntRetTupleIntInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2, 2)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleFloatFloatRetTupleFloatFloat() { 
    bool compStatus = compileChecker("func/funcTupleFloatFloatRetTupleFloatFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2.000000, 2.000000)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleBoolBoolRetTupleBoolBool() { 
    bool compStatus = compileChecker("func/funcTupleBoolBoolRetTupleBoolBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(True, False)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleCharCharRetTupleCharChar() { 
    bool compStatus = compileChecker("func/funcTupleCharCharRetTupleCharChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "('c', 'h')");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleStringStringRetTupleStringString() { 
    bool compStatus = compileChecker("func/funcTupleStringStringRetTupleStringString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(\"string\", \"string\")");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntToIntRetInt() { 
    bool compStatus = compileChecker("func/funcIntToIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcFloatToFloatRetFloat() { 
    bool compStatus = compileChecker("func/funcFloatToFloatRetFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcBoolToBoolRetBool() { 
    bool compStatus = compileChecker("func/funcBoolToBoolRetBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcCharToCharRetChar() { 
    bool compStatus = compileChecker("func/funcCharToCharRetChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcStringToStringRetString() { 
    bool compStatus = compileChecker("func/funcStringToStringRetString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcListIntToListIntRetListInt() { 
    bool compStatus = compileChecker("func/funcListIntToListIntRetListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcTupleIntIntToTupleIntIntRetInt() { 
    bool compStatus = compileChecker("func/funcTupleIntIntToTupleIntIntRetInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(1, 2)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::funcIntToIntRetToIntRetToInt() { 
    bool compStatus = compileChecker("func/funcIntToIntRetToIntRetToInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

