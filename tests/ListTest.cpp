#include "Test.h"


void Test::listCasEmpty() { 
    bool compStatus = compileChecker("listCasEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendEmpty() { 
    bool compStatus = compileChecker("listAppendEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listInt() { 
    bool compStatus = compileChecker("listInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[2]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloat() { 
    bool compStatus = compileChecker("listFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[2.000000]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBool() { 
    bool compStatus = compileChecker("listBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[True]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listChar() { 
    bool compStatus = compileChecker("listChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "['c']");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listString() { 
    bool compStatus = compileChecker("listString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[\"string\"]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListInt() { 
    bool compStatus = compileChecker("listListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[2]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListFloat() { 
    bool compStatus = compileChecker("listListFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[2.000000]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListBool() { 
    bool compStatus = compileChecker("listListBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[True]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListChar() { 
    bool compStatus = compileChecker("listListChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[['c']]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListString() { 
    bool compStatus = compileChecker("listListString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[\"string\"]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listListListInt() { 
    bool compStatus = compileChecker("listListListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[[2]]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendInt() { 
    bool compStatus = compileChecker("listAppendInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1, 2, 3]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendFloat() { 
    bool compStatus = compileChecker("listAppendFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[1.000000, 2.000000, 3.000000]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendBool() { 
    bool compStatus = compileChecker("listAppendBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[True, False, True]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendChar() { 
    bool compStatus = compileChecker("listAppendChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "['c', 'h', 'a', 'r']");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listAppendString() { 
    bool compStatus = compileChecker("listAppendString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[\"string\", \"string\", \"string\"]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listLongInt() { 
    bool compStatus = compileChecker("listLongInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInInt() { 
    bool compStatus = compileChecker("listFloatInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInInt() { 
    bool compStatus = compileChecker("listBoolInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInInt() { 
    bool compStatus = compileChecker("listCharInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInInt() { 
    bool compStatus = compileChecker("listStringInInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInFloat() { 
    bool compStatus = compileChecker("listIntInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInFloat() { 
    bool compStatus = compileChecker("listBoolInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInFloat() { 
    bool compStatus = compileChecker("listCharInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInFloat() { 
    bool compStatus = compileChecker("listStringInFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInBool() { 
    bool compStatus = compileChecker("listIntInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInBool() { 
    bool compStatus = compileChecker("listFloatInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInBool() { 
    bool compStatus = compileChecker("listCharInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInBool() { 
    bool compStatus = compileChecker("listStringInBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInChar() { 
    bool compStatus = compileChecker("listIntInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInChar() { 
    bool compStatus = compileChecker("listFloatInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInChar() { 
    bool compStatus = compileChecker("listBoolInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listStringInChar() { 
    bool compStatus = compileChecker("listStringInChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listIntInString() { 
    bool compStatus = compileChecker("listIntInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listFloatInString() { 
    bool compStatus = compileChecker("listFloatInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listBoolInString() { 
    bool compStatus = compileChecker("listBoolInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listCharInString() { 
    bool compStatus = compileChecker("listCharInString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listMixTypes() { 
    bool compStatus = compileChecker("listMixTypes.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listNestedDifLengthInt() { 
    bool compStatus = compileChecker("listNestedDifLengthInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[[1, 2, 3], [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleInt() { 
    bool compStatus = compileChecker("listTupleInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(1, 2), (3, 4), (5, 6)]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleFloat() { 
    bool compStatus = compileChecker("listTupleFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(1.000000, 2.000000), (3.000000, 4.000000), (5.000000, 6.000000)]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleBool() { 
    bool compStatus = compileChecker("listTupleBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(True, False), (False, True), (False, False)]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleChar() { 
    bool compStatus = compileChecker("listTupleChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[('c', 'h'), ('a', 'r'), ('c', 'h')]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::listTupleString() { 
    bool compStatus = compileChecker("listTupleString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "[(\"string\", \"string\"), (\"string\", \"string\"), (\"string\", \"string\")]");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

