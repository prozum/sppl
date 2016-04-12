#include "Test.h"


void Test::sigLengthZeroInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int", "", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthOneInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int", " a", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthTwoInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int", " a b", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthThreeInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int", " a b c", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthFourInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int", " a b c d", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthFiveInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int->Int", " a b c d e", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthSixInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int->Int->Int", " a b c d e f", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthSevenInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int->Int->Int->Int", " a b c d e f g", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthEightInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int->Int->Int->Int->Int", " a b c d e f g h", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthNineInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int->Int->Int->Int->Int->Int", " a b c d e f g h i", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::sigLengthTenInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int->Int->Int->Int->Int->Int->Int->Int->Int", " a b c d e f g h i j", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::patLengthSameInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int", " a b", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "2");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::patLengthLessInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int", " a", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::patLengthMoreInt() {
    shared_ptr<std::stringstream> source = buildSimple("Int->Int->Int", " a b c", "2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

