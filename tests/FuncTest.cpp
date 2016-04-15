#include "Test.h"


void Test::funcRetInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("Int",
        "Int", "", "2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcRetFloat () {
    std::shared_ptr<std::stringstream> source = buildFunc("Float",
        "Float", "", "2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcRetBool () {
    std::shared_ptr<std::stringstream> source = buildFunc("Bool",
        "Bool", "", "True", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcRetChar () {
    std::shared_ptr<std::stringstream> source = buildFunc("Char",
        "Char", "", "'c'", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcRetString () {
    std::shared_ptr<std::stringstream> source = buildFunc("String",
        "String", "", "\"string\"", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcIntRetInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("Int",
        "Int->Int", "n", "n", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcFloatRetFloat () {
    std::shared_ptr<std::stringstream> source = buildFunc("Float",
        "Float->Float", "n", "n", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcBoolRetBool () {
    std::shared_ptr<std::stringstream> source = buildFunc("Bool",
        "Bool->Bool", "n", "n", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcCharRetChar () {
    std::shared_ptr<std::stringstream> source = buildFunc("Char",
        "Char->Char", "n", "n", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcStringRetString () {
    std::shared_ptr<std::stringstream> source = buildFunc("String",
        "String->String", "n", "n", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcIntIntRetInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("Int",
        "Int->Int->Int", "m n", "m+n", "2,2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcFloatFloatRetFloat () {
    std::shared_ptr<std::stringstream> source = buildFunc("Float",
        "Float->Float->Float", "m n", "m+n", "2.0,2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "4.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcBoolBoolRetBool () {
    std::shared_ptr<std::stringstream> source = buildFunc("Bool",
        "Bool->Bool->Bool", "m n", "m||n", "True,False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcCharCharRetChar () {
    std::shared_ptr<std::stringstream> source = buildFunc("Char",
        "Char->Char->Char", "m n", "'c'", "'c','h'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcStringStringRetString () {
    std::shared_ptr<std::stringstream> source = buildFunc("String",
        "String->String->String", "m n", "\"string\"", "\"string\", \"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcListIntRetListInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("[Int]",
        "[Int]->[Int]", "n", "n", "[1]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcListFloatRetListFloat () {
    std::shared_ptr<std::stringstream> source = buildFunc("[Float]",
        "[Float]->[Float]", "n", "n", "[1.0]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[1.000000]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcListBoolRetListBool () {
    std::shared_ptr<std::stringstream> source = buildFunc("[Bool]",
        "[Bool]->[Bool]", "n", "n", "[True]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[True]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcListCharRetListChar () {
    std::shared_ptr<std::stringstream> source = buildFunc("[Char]",
        "[Char]->[Char]", "n", "n", "['c']");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "['c']");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcListStringRetListString () {
    std::shared_ptr<std::stringstream> source = buildFunc("[String]",
        "[String]->[String]", "n", "n", "[\"string\"]");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[\"string\"]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcTupleIntIntRetTupleIntInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("(Int,Int)",
        "(Int,Int)->(Int,Int)", "n", "n", "(2,2)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2, 2)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcTupleFloatFloatRetTupleFloatFloat () {
    std::shared_ptr<std::stringstream> source = buildFunc("(Float,Float)",
        "(Float,Float)->(Float,Float)", "n", "n", "(2.0,2.0)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(2.000000, 2.000000)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcTupleBoolBoolRetTupleBoolBool () {
    std::shared_ptr<std::stringstream> source = buildFunc("(Bool,Bool)",
        "(Bool,Bool)->(Bool,Bool)", "n", "n", "(True,False)");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(True, False)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcTupleCharCharRetTupleCharChar () {
    std::shared_ptr<std::stringstream> source = buildFunc("(Char,Char)",
        "(Char,Char)->(Char,Char)", "n", "n", "('c','h')");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "('c', 'h')");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcTupleStringStringRetTupleStringString () {
    std::shared_ptr<std::stringstream> source = buildFunc("(String,String)",
        "(String,String)->(String,String)", "n", "n", "(\"string\",\"string\")");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(\"string\", \"string\")");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcIntToIntRetInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("Int",
        "(Int->Int)->Int", "f", "f(2)", "func2",
        "Int->Int", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcFloatToFloatRetFloat () {
    std::shared_ptr<std::stringstream> source = buildFunc("Float",
        "(Float->Float)->Float", "f", "f(2.0)", "func2",
        "Float->Float", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcBoolToBoolRetBool () {
    std::shared_ptr<std::stringstream> source = buildFunc("Bool",
        "(Bool->Bool)->Bool", "f", "f(True)", "func2",
        "Bool->Bool", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcCharToCharRetChar () {
    std::shared_ptr<std::stringstream> source = buildFunc("Char",
        "(Char->Char)->Char", "f", "f('c')", "func2",
        "Char->Char", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "'c'");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcStringToStringRetString () {
    std::shared_ptr<std::stringstream> source = buildFunc("String",
        "(String->String)->String", "f", "f(\"string\")", "func2",
        "String->String", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "\"string\"");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcListIntToListIntRetListInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("[Int]",
        "([Int]->[Int])->[Int]", "f", "f([1])", "func2",
        "[Int]->[Int]", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "[1]");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcTupleIntIntToTupleIntIntRetInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("(Int,Int)",
        "((Int,Int)->(Int,Int))->(Int,Int)", "f", "f((1,2))", "func2",
        "(Int,Int)->(Int,Int)", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "(1, 2)");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


void Test::funcIntToIntRetToIntRetToInt () {
    std::shared_ptr<std::stringstream> source = buildFunc("Int",
        "((Int->Int)->Int)->Int", "f", "f(func3)", "func2",
        "(Int->Int)->Int", "f", "f(2)",
        "Int->Int", "n", "n");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}


