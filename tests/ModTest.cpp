#include "Test.h"


void Test::modIntPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2%2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modIntZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","0%0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modIntPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2%-2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modIntNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2%2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modIntNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2%-2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modFloatPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0%2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modFloatZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","0.0%0.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modFloatPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0%-2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modFloatNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0%2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modFloatNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0%-2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.000000");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modBoolTrueTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True%True", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modBoolTrueFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True%False", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modBoolFalseTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False%True", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modBoolFalseFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False%False", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modCharStrStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"%\"string\"", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modCharStrCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"%'c'", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modCharChaStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'%\"string\"", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modCharChaCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'%'c'", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modMixFloatInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2-0%2", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::modMixIntFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2%2.0", "");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

