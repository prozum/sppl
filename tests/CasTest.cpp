#include "Test.h"


void Test::casCountOneInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountTwoInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountThreeInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountFourInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountFiveInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountSixInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountSevenInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountEightInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountNineInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    pattern.push_back("8");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::casCountTenInt() {
    std::vector<std::string> pattern;
    pattern.push_back("0");
    pattern.push_back("1");
    pattern.push_back("2");
    pattern.push_back("3");
    pattern.push_back("4");
    pattern.push_back("5");
    pattern.push_back("6");
    pattern.push_back("7");
    pattern.push_back("8");
    pattern.push_back("9");
    std::shared_ptr<std::stringstream> source = buildMultiCase("Int", "Int", pattern, "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if(compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

