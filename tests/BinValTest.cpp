#include "Test.h"


void Test::binEqualIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2==2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binNotEqualIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2!=2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binAndIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2&&2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binOrIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2||2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binGrEqIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2>=2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binLeEqIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2<=2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binGreatIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2>2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binLessIntInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2<2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binEqualFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0==2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binNotEqualFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0!=2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binAndFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0&&2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binOrFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0||2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binGrEqFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0>=2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binLeEqFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0<=2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "True");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binGreatFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0>2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::binLessFloatFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","2.0<2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "False");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

