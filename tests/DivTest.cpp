#include "Test.h"


void Test::divIntPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2/2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "1");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divIntZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","0/0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divIntPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2/-2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-1");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divIntNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2/2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-1");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divIntNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2/-2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "1");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divFloatPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0/2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "1.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divFloatZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","0.0/0.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divFloatPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0/-2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-1.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divFloatNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0/2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-1.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divFloatNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0/-2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "1.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divBoolTrueTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True/True", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divBoolTrueFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True/False", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divBoolFalseTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False/True", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divBoolFalseFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False/False", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divCharStrStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"/\"string\"", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divCharStrCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"/'c'", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divCharChaStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'/\"string\"", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divCharChaCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'/'c'", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divMixFloatInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2-0/2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::divMixIntFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2/2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

