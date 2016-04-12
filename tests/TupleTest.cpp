#include "Test.h"


void Test::tupleEmpty() {
    std::shared_ptr<std::stringstream> source = buildSimple("()", "", "()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleEmptyInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Int)", "", "()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleEmptyFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Float)", "", "()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleEmptyBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Bool)", "", "()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleEmptyChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Char)", "", "()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleEmptyString() {
    std::shared_ptr<std::stringstream> source = buildSimple("(String)", "", "()");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleOneInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Int)", "", "(2)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleOneFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Float)", "", "(2.0)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleOneBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Bool)", "", "(True)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleOneChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Char)", "", "('c')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleOneString() {
    std::shared_ptr<std::stringstream> source = buildSimple("(String)", "", "(\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleTwoInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Int,Int)", "", "(2,2)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2,2)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleTwoFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Float,Float)", "", "(2.0,2.0)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2.0,2.0)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleTwoBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Bool,Bool)", "", "(True,False)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(True,False)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleTwoChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Char, Char)", "", "('c','h')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "('c','h')");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleTwoString() {
    std::shared_ptr<std::stringstream> source = buildSimple("(String, String)", "", "(\"string\",\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(\"string\",\"string\")");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleThreeInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Int,Int, Int)", "", "(2,2,2)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2,2,2)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleThreeFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Float,Float, Float)", "", "(2.0,2.0,2.0)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2.0,2.0,2.0)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleThreeBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Bool,Bool, Bool)", "", "(True, False, True)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(True, False, True)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleThreeChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Char, Char, Char)", "", "('c','h','a')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "('c','h','a')");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleThreeString() {
    std::shared_ptr<std::stringstream> source = buildSimple("(String, String, String)", "", "(\"string\",\"string\",\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(\"string\",\"string\",\"string\")");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleMixed() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Int, Float, Bool, Char, Char, Char, Char, String)", "", "(2,2.0,True,'c','h','a','r',\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2,2.0,True,'c','h','a','r',\"string\")");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleListInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("([Int],[Int])", "", "([2],[2])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "([2],[2])");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleListFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("([Float],[Float])", "", "([2.0],[2.0])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "([2.0],[2.0])");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleListBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("([Bool],[Bool])", "", "([True],[False])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "([True],[False])");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleListChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("([Char],[Char])", "", "(['c'],['h'])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(['c'],['h'])");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleListString() {
    std::shared_ptr<std::stringstream> source = buildSimple("([String],[String])", "", "([\"string\"],[\"string\"])");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "([\"string\"],[\"string\"])");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleNested() {
    std::shared_ptr<std::stringstream> source = buildSimple("((Int, Int), (Int,Int))", "", "((1,2),(3,4))");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "((1,2),(3,4))");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleTwoNested() {
    std::shared_ptr<std::stringstream> source = buildSimple("((Int, Float), (Bool, (Char, String)))", "", "((1,2.0),(True,('c',\"har\")))");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "((1,2.0),(True,('c',\"har\")))");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleSuperNested() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,Char)))))))))))", "", "('h',('e',('l',('l',('o',(' ',('w',('o',('r',('l',('d','!')))))))))))");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "('h',('e',('l',('l',('o',(' ',('w',('o',('r',('l',('d','!')))))))))))");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::tupleWrongType() {
    std::shared_ptr<std::stringstream> source = buildSimple("(Int,Float,Bool,Char,String)", "", "(1.2,True,'c',\"string\",1)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

