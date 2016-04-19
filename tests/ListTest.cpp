#include "Test.h"


void Test::listCasEmpty() { 
    bool compStatus = compileChecker("list/listCasEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendEmpty() { 
    bool compStatus = compileChecker("list/listAppendEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listInt() { 
    bool compStatus = compileChecker("list/listInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[2]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloat() { 
    bool compStatus = compileChecker("list/listFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[2.000000]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBool() { 
    bool compStatus = compileChecker("list/listBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[True]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listChar() { 
    bool compStatus = compileChecker("list/listChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "['c']");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listString() { 
    bool compStatus = compileChecker("list/listString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[\"string\"]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListInt() { 
    bool compStatus = compileChecker("list/listListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[2]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListFloat() { 
    bool compStatus = compileChecker("list/listListFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[2.000000]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListBool() { 
    bool compStatus = compileChecker("list/listListBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[True]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListChar() { 
    bool compStatus = compileChecker("list/listListChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[['c']]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListString() { 
    bool compStatus = compileChecker("list/listListString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[\"string\"]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListListInt() { 
    bool compStatus = compileChecker("list/listListListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[[2]]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendInt() { 
    bool compStatus = compileChecker("list/listAppendInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1, 2, 3]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendFloat() { 
    bool compStatus = compileChecker("list/listAppendFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1.000000, 2.000000, 3.000000]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendBool() { 
    bool compStatus = compileChecker("list/listAppendBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[True, False, True]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendChar() { 
    bool compStatus = compileChecker("list/listAppendChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "['c', 'h', 'a', 'r']");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendString() { 
    bool compStatus = compileChecker("list/listAppendString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[\"string\", \"string\", \"string\"]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listLongInt() { 
    bool compStatus = compileChecker("list/listLongInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInInt() { 
    bool compStatus = compileChecker("list/listFloatInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInInt() { 
    bool compStatus = compileChecker("list/listBoolInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInInt() { 
    bool compStatus = compileChecker("list/listCharInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInInt() { 
    bool compStatus = compileChecker("list/listStringInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInFloat() { 
    bool compStatus = compileChecker("list/listIntInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInFloat() { 
    bool compStatus = compileChecker("list/listBoolInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInFloat() { 
    bool compStatus = compileChecker("list/listCharInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInFloat() { 
    bool compStatus = compileChecker("list/listStringInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInBool() { 
    bool compStatus = compileChecker("list/listIntInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInBool() { 
    bool compStatus = compileChecker("list/listFloatInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInBool() { 
    bool compStatus = compileChecker("list/listCharInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInBool() { 
    bool compStatus = compileChecker("list/listStringInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInChar() { 
    bool compStatus = compileChecker("list/listIntInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInChar() { 
    bool compStatus = compileChecker("list/listFloatInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInChar() { 
    bool compStatus = compileChecker("list/listBoolInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInChar() { 
    bool compStatus = compileChecker("list/listStringInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInString() { 
    bool compStatus = compileChecker("list/listIntInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInString() { 
    bool compStatus = compileChecker("list/listFloatInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInString() { 
    bool compStatus = compileChecker("list/listBoolInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInString() { 
    bool compStatus = compileChecker("list/listCharInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listMixTypes() { 
    bool compStatus = compileChecker("list/listMixTypes.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listNestedDifLengthInt() { 
    bool compStatus = compileChecker("list/listNestedDifLengthInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[1, 2, 3], [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleInt() { 
    bool compStatus = compileChecker("list/listTupleInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(1, 2), (3, 4), (5, 6)]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleFloat() { 
    bool compStatus = compileChecker("list/listTupleFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(1.000000, 2.000000), (3.000000, 4.000000), (5.000000, 6.000000)]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleBool() { 
    bool compStatus = compileChecker("list/listTupleBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(True, False), (False, True), (False, False)]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleChar() { 
    bool compStatus = compileChecker("list/listTupleChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[('c', 'h'), ('a', 'r'), ('c', 'h')]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleString() { 
    bool compStatus = compileChecker("list/listTupleString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(\"string\", \"string\"), (\"string\", \"string\"), (\"string\", \"string\")]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

