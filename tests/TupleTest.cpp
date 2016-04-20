#include "Test.h"


void Test::tupleEmpty() { 
    bool compStatus = compileChecker("tuple/tupleEmpty.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleEmptyInt() { 
    bool compStatus = compileChecker("tuple/tupleEmptyInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleEmptyFloat() { 
    bool compStatus = compileChecker("tuple/tupleEmptyFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleEmptyBool() { 
    bool compStatus = compileChecker("tuple/tupleEmptyBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleEmptyChar() { 
    bool compStatus = compileChecker("tuple/tupleEmptyChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleEmptyString() { 
    bool compStatus = compileChecker("tuple/tupleEmptyString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleOneInt() { 
    bool compStatus = compileChecker("tuple/tupleOneInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleOneFloat() { 
    bool compStatus = compileChecker("tuple/tupleOneFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleOneBool() { 
    bool compStatus = compileChecker("tuple/tupleOneBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleOneChar() { 
    bool compStatus = compileChecker("tuple/tupleOneChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleOneString() { 
    bool compStatus = compileChecker("tuple/tupleOneString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}

void Test::tupleTwoInt() { 
    bool compStatus = compileChecker("tuple/tupleTwoInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(2, 2)");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleTwoFloat() { 
    bool compStatus = compileChecker("tuple/tupleTwoFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(2.000000, 2.000000)");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleTwoBool() { 
    bool compStatus = compileChecker("tuple/tupleTwoBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(True, False)");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleTwoChar() { 
    bool compStatus = compileChecker("tuple/tupleTwoChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "('c', 'h')");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleTwoString() { 
    bool compStatus = compileChecker("tuple/tupleTwoString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(\"string\", \"string\")");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleThreeInt() { 
    bool compStatus = compileChecker("tuple/tupleThreeInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(2, 2, 2)");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleThreeFloat() { 
    bool compStatus = compileChecker("tuple/tupleThreeFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(2.000000, 2.000000, 2.000000)");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleThreeBool() { 
    bool compStatus = compileChecker("tuple/tupleThreeBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(True, False, True)");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleThreeChar() { 
    bool compStatus = compileChecker("tuple/tupleThreeChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "('c', 'h', 'a')");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleThreeString() { 
    bool compStatus = compileChecker("tuple/tupleThreeString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(\"string\", \"string\", \"string\")");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleMixed() { 
    bool compStatus = compileChecker("tuple/tupleMixed.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(2, 2.000000, True, 'c', 'h', 'a', 'r', \"string\")");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleListInt() { 
    bool compStatus = compileChecker("tuple/tupleListInt.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "([2], [2])");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleListFloat() { 
    bool compStatus = compileChecker("tuple/tupleListFloat.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "([2.000000], [2.000000])");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleListBool() { 
    bool compStatus = compileChecker("tuple/tupleListBool.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "([True], [False])");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleListChar() { 
    bool compStatus = compileChecker("tuple/tupleListChar.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "(['c'], ['h'])");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleListString() { 
    bool compStatus = compileChecker("tuple/tupleListString.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "([\"string\"], [\"string\"])");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleNested() { 
    bool compStatus = compileChecker("tuple/tupleNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "((1, 2), (3, 4))");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleTwoNested() { 
    bool compStatus = compileChecker("tuple/tupleTwoNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "((1, 2.000000), (True, ('c', \"har\")))");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleSuperNested() { 
    bool compStatus = compileChecker("tuple/tupleSuperNested.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgFail, compStatus);
    bool execStatus = executeChecker("", "('h', ('e', ('l', ('l', ('o', (' ', ('w', ('o', ('r', ('l', ('d', '!')))))))))))");
    CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);
}

void Test::tupleWrongType() { 
    bool compStatus = compileChecker("tuple/tupleWrongType.sppl");
    CPPUNIT_ASSERT_MESSAGE(compMsgSucc, !compStatus);
}