#include "Test.h"


void Test::tupleEmpty() { 
    bool compStatus = compileChecker("tupleEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleEmptyInt() { 
    bool compStatus = compileChecker("tupleEmptyInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleEmptyFloat() { 
    bool compStatus = compileChecker("tupleEmptyFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleEmptyBool() { 
    bool compStatus = compileChecker("tupleEmptyBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleEmptyChar() { 
    bool compStatus = compileChecker("tupleEmptyChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleEmptyString() { 
    bool compStatus = compileChecker("tupleEmptyString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleOneInt() { 
    bool compStatus = compileChecker("tupleOneInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleOneFloat() { 
    bool compStatus = compileChecker("tupleOneFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleOneBool() { 
    bool compStatus = compileChecker("tupleOneBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleOneChar() { 
    bool compStatus = compileChecker("tupleOneChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleOneString() { 
    bool compStatus = compileChecker("tupleOneString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleTwoInt() { 
    bool compStatus = compileChecker("tupleTwoInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2, 2)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleTwoFloat() { 
    bool compStatus = compileChecker("tupleTwoFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2.000000, 2.000000)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleTwoBool() { 
    bool compStatus = compileChecker("tupleTwoBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(True, False)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleTwoChar() { 
    bool compStatus = compileChecker("tupleTwoChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "('c', 'h')");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleTwoString() { 
    bool compStatus = compileChecker("tupleTwoString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(\"string\", \"string\")");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleThreeInt() { 
    bool compStatus = compileChecker("tupleThreeInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2, 2, 2)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleThreeFloat() { 
    bool compStatus = compileChecker("tupleThreeFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2.000000, 2.000000, 2.000000)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleThreeBool() { 
    bool compStatus = compileChecker("tupleThreeBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(True, False, True)");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleThreeChar() { 
    bool compStatus = compileChecker("tupleThreeChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "('c', 'h', 'a')");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleThreeString() { 
    bool compStatus = compileChecker("tupleThreeString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(\"string\", \"string\", \"string\")");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleMixed() { 
    bool compStatus = compileChecker("tupleMixed.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(2, 2.000000, True, 'c', 'h', 'a', 'r', \"string\")");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleListInt() { 
    bool compStatus = compileChecker("tupleListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "([2], [2])");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleListFloat() { 
    bool compStatus = compileChecker("tupleListFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "([2.000000], [2.000000])");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleListBool() { 
    bool compStatus = compileChecker("tupleListBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "([True], [False])");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleListChar() { 
    bool compStatus = compileChecker("tupleListChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "(['c'], ['h'])");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleListString() { 
    bool compStatus = compileChecker("tupleListString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "([\"string\"], [\"string\"])");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleNested() { 
    bool compStatus = compileChecker("tupleNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "((1, 2), (3, 4))");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleTwoNested() { 
    bool compStatus = compileChecker("tupleTwoNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "((1, 2.000000), (True, ('c', \"har\")))");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleSuperNested() { 
    bool compStatus = compileChecker("tupleSuperNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "('h', ('e', ('l', ('l', ('o', (' ', ('w', ('o', ('r', ('l', ('d', '!')))))))))))");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

void Test::tupleWrongType() { 
    bool compStatus = compileChecker("tupleWrongType.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
    }
}

