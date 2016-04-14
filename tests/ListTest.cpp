#include "Test.h"


void Test::listCasEmpty() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]->Int", "[]", "2", "[]");
    bool compStatus = compileChecker(source);
    string s = source->str();
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listAppendEmpty() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "1:[]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "[2]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[2]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Float]", "", "[2.0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[2.000000]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Bool]", "", "[True]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[True]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Char]", "", "['c']", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "['c']");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[String]", "", "[\"string\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[\"string\"]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listListInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[Int]]", "", "[[2]]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[[2]]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listListFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[Float]]", "", "[[2.0]]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[[2.000000]]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listListBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[Bool]]", "", "[[True]]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[[True]]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listListChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[Char]]", "", "[['c']]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[['c']]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listListString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[String]]", "", "[[\"string\"]]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[[\"string\"]]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listListListInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[[Int]]]", "", "[[[2]]]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[[[2]]]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listAppendInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "1:[2,3]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[1, 2, 3]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listAppendFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Float]", "", "1.0:[2.0,3.0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[1.000000, 2.000000, 3.000000]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listAppendBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Bool]", "", "True:[False,True]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[True, False, True]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listAppendChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Char]", "", "'c':['h','a','r']", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "['c', 'h', 'a', 'r']");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listAppendString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[String]", "", "\"string\":[\"string\",\"string\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[\"string\", \"string\", \"string\"]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listLongInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "[0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listFloatInInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "[2.0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listBoolInInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "[True]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listCharInInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "['c']", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listStringInInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "[\"string\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listIntInFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Float]", "", "[2]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listBoolInFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Float]", "", "[True]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listCharInFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Float]", "", "['c']", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listStringInFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Float]", "", "[\"string\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listIntInBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Bool]", "", "[2]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listFloatInBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Bool]", "", "[2.0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listCharInBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Bool]", "", "['c']", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listStringInBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Bool]", "", "[\"string\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listIntInChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Char]", "", "[2]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listFloatInChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Char]", "", "[2.0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listBoolInChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Char]", "", "[True]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listStringInChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Char]", "", "[\"True\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listIntInString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[String]", "", "[2]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listFloatInString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[String]", "", "[2.0]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listBoolInString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[String]", "", "[True]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listCharInString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[String]", "", "['c']", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listMixTypes() {
    std::shared_ptr<std::stringstream> source = buildSimple("[Int]", "", "[2,2.0,False,'c',\"string\"]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listNestedDifLengthInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[[Int]]", "", "[[1,2,3],[1,2,3,4,5,6,7,8,9,0]]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[[1, 2, 3], [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listTupleInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("[(Int, Int)]", "", "[(1,2),(3,4),(5,6)]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[(1, 2), (3, 4), (5, 6)]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listTupleFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("[(Float, Float)]", "", "[(1.0,2.0),(3.0,4.0),(5.0,6.0)]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[(1.000000, 2.000000), (3.000000, 4.000000), (5.000000, 6.000000)]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listTupleBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("[(Bool, Bool)]", "", "[(True,False),(False,True),(False,False)]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[(True, False), (False, True), (False, False)]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listTupleChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("[(Char, Char)]", "", "[('c','h'),('a','r'),('c','h')]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[('c', 'h'), ('a', 'r'), ('c', 'h')]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::listTupleString() {
    std::shared_ptr<std::stringstream> source = buildSimple("[(String, String)]", "", "[(\"string\",\"string\"),(\"string\",\"string\"),(\"string\",\"string\")]", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[(\"string\", \"string\"), (\"string\", \"string\"), (\"string\", \"string\")]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

