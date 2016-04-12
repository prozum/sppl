#include "Test.h"


void Test::subIntPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subIntZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","0-0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subIntPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2--2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subIntNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subIntNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2--2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subFloatPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subFloatZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","0.0-0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subFloatPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0--2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subFloatNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subFloatNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0--2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subBoolTrueTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True-True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subBoolTrueFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True-False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subBoolFalseTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False-True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subBoolFalseFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False-False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subCharStrStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"-\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subCharStrCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"-'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subCharChaStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'-\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subCharChaCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'-'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subMixFloatInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2-0-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::subMixIntFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

