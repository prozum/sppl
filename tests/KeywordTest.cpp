#include "Test.h"


void Test::keywordInt() {
    std::shared_ptr<std::stringstream> source = buildSimple("Int->Int", "Int", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::keywordFloat() {
    std::shared_ptr<std::stringstream> source = buildSimple("Float->Float", "Float", "2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::keywordBool() {
    std::shared_ptr<std::stringstream> source = buildSimple("Bool->Bool", "Bool", "True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::keywordChar() {
    std::shared_ptr<std::stringstream> source = buildSimple("Char->Char", "Char", "'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::keywordString() {
    std::shared_ptr<std::stringstream> source = buildSimple("String->String", "String", "\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::keywordDef() {
    std::shared_ptr<std::stringstream> source = buildSimple("Int->Int", "def", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::keywordMain() {
    std::shared_ptr<std::stringstream> source = buildSimple("Int->Int", "main", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if(compStatus) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::comment() {
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "", "2 # This is a comment");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

