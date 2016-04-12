#include "Test.h"


void Test::mulIntPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2*2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulIntZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","0*0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulIntPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2*-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulIntNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2*2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulIntNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","-2*-2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulFloatPosPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0*2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulFloatZeroZero() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","0.0*0.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "0.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulFloatPosNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2.0*-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulFloatNegPos() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0*2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "-4.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulFloatNegNeg() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","-2.0*-2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == true);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "4.0");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulBoolTrueTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True*True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulBoolTrueFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","True*False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulBoolFalseTrue() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False*True");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulBoolFalseFalse() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Bool", "","False*False");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulCharStrStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"*\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulCharStrCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("String", "","\"string\"*'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulCharChaStr() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'*\"string\"");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulCharChaCha() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Char", "","'c'*'c'");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulMixFloatInt() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Float", "","2-0*2");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

void Test::mulMixIntFloat() { 
    std::shared_ptr<std::stringstream> source = buildSimple("Int", "","2*2.0");
    bool compStatus = compileChecker(source);
    CPPUNIT_ASSERT_MESSAGE("Compilation failed", compStatus == false);
    if (compStatus == true) {
        bool execStatus = executeChecker("", "");
        CPPUNIT_ASSERT_MESSAGE("Execution failed", execStatus);
    }
}

