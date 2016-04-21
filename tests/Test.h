#pragma once

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include "Compiler.h"
#include <fstream>
#include <cstring>
#include <iostream>

class Test : public CppUnit::TestFixture{
    CPPUNIT_TEST_SUITE(Test);
    // AddTest.cpp
        CPPUNIT_TEST(addIntPosPos);
        CPPUNIT_TEST(addIntZeroZero);
        CPPUNIT_TEST(addIntPosNeg);
        CPPUNIT_TEST(addIntNegPos);
        CPPUNIT_TEST(addIntNegNeg);
        CPPUNIT_TEST(addFloatPosPos);
        CPPUNIT_TEST(addFloatZeroZero);
        CPPUNIT_TEST(addFloatPosNeg);
        CPPUNIT_TEST(addFloatNegPos);
        CPPUNIT_TEST(addFloatNegNeg);
        CPPUNIT_TEST(addBoolTrueTrue);
        CPPUNIT_TEST(addBoolTrueFalse);
        CPPUNIT_TEST(addBoolFalseTrue);
        CPPUNIT_TEST(addBoolFalseFalse);
        CPPUNIT_TEST(addCharStrStr);
        CPPUNIT_TEST(addCharStrCha);
        CPPUNIT_TEST(addCharChaStr);
        CPPUNIT_TEST(addCharChaCha);
        CPPUNIT_TEST(addMixFloatInt);
        CPPUNIT_TEST(addMixIntFloat);

    // SubTest.cpp
        CPPUNIT_TEST(subIntPosPos);
        CPPUNIT_TEST(subIntZeroZero);
        CPPUNIT_TEST(subIntPosNeg);
        CPPUNIT_TEST(subIntNegPos);
        CPPUNIT_TEST(subIntNegNeg);
        CPPUNIT_TEST(subFloatPosPos);
        CPPUNIT_TEST(subFloatZeroZero);
        CPPUNIT_TEST(subFloatPosNeg);
        CPPUNIT_TEST(subFloatNegPos);
        CPPUNIT_TEST(subFloatNegNeg);
        CPPUNIT_TEST(subBoolTrueTrue);
        CPPUNIT_TEST(subBoolTrueFalse);
        CPPUNIT_TEST(subBoolFalseTrue);
        CPPUNIT_TEST(subBoolFalseFalse);
        CPPUNIT_TEST(subCharStrStr);
        CPPUNIT_TEST(subCharStrCha);
        CPPUNIT_TEST(subCharChaStr);
        CPPUNIT_TEST(subCharChaCha);
        CPPUNIT_TEST(subMixFloatInt);
        CPPUNIT_TEST(subMixIntFloat);
        CPPUNIT_TEST(subBoolNegative);

    // MulTest.cpp
        CPPUNIT_TEST(mulIntPosPos);
        CPPUNIT_TEST(mulIntZeroZero);
        CPPUNIT_TEST(mulIntPosNeg);
        CPPUNIT_TEST(mulIntNegPos);
        CPPUNIT_TEST(mulIntNegNeg);
        CPPUNIT_TEST(mulFloatPosPos);
        CPPUNIT_TEST(mulFloatZeroZero);
        CPPUNIT_TEST(mulFloatPosNeg);
        CPPUNIT_TEST(mulFloatNegPos);
        CPPUNIT_TEST(mulFloatNegNeg);
        CPPUNIT_TEST(mulBoolTrueTrue);
        CPPUNIT_TEST(mulBoolTrueFalse);
        CPPUNIT_TEST(mulBoolFalseTrue);
        CPPUNIT_TEST(mulBoolFalseFalse);
        CPPUNIT_TEST(mulCharStrStr);
        CPPUNIT_TEST(mulCharStrCha);
        CPPUNIT_TEST(mulCharChaStr);
        CPPUNIT_TEST(mulCharChaCha);
        CPPUNIT_TEST(mulMixFloatInt);
        CPPUNIT_TEST(mulMixIntFloat);

    // DivTest.cpp
        CPPUNIT_TEST(divIntPosPos);
        CPPUNIT_TEST(divIntZeroZero);
        CPPUNIT_TEST(divIntPosNeg);
        CPPUNIT_TEST(divIntNegPos);
        CPPUNIT_TEST(divIntNegNeg);
        CPPUNIT_TEST(divFloatPosPos);
        CPPUNIT_TEST(divFloatZeroZero);
        CPPUNIT_TEST(divFloatPosNeg);
        CPPUNIT_TEST(divFloatNegPos);
        CPPUNIT_TEST(divFloatNegNeg);
        CPPUNIT_TEST(divBoolTrueTrue);
        CPPUNIT_TEST(divBoolTrueFalse);
        CPPUNIT_TEST(divBoolFalseTrue);
        CPPUNIT_TEST(divBoolFalseFalse);
        CPPUNIT_TEST(divCharStrStr);
        CPPUNIT_TEST(divCharStrCha);
        CPPUNIT_TEST(divCharChaStr);
        CPPUNIT_TEST(divCharChaCha);
        CPPUNIT_TEST(divMixFloatInt);
        CPPUNIT_TEST(divMixIntFloat);

    // ModTest.cpp
        CPPUNIT_TEST(modIntPosPos);
        CPPUNIT_TEST(modIntZeroZero);
        CPPUNIT_TEST(modIntPosNeg);
        CPPUNIT_TEST(modIntNegPos);
        CPPUNIT_TEST(modIntNegNeg);
        CPPUNIT_TEST(modFloatPosPos);
        CPPUNIT_TEST(modFloatZeroZero);
        CPPUNIT_TEST(modFloatPosNeg);
        CPPUNIT_TEST(modFloatNegPos);
        CPPUNIT_TEST(modFloatNegNeg);
        CPPUNIT_TEST(modBoolTrueTrue);
        CPPUNIT_TEST(modBoolTrueFalse);
        CPPUNIT_TEST(modBoolFalseTrue);
        CPPUNIT_TEST(modBoolFalseFalse);
        CPPUNIT_TEST(modCharStrStr);
        CPPUNIT_TEST(modCharStrCha);
        CPPUNIT_TEST(modCharChaStr);
        CPPUNIT_TEST(modCharChaCha);
        CPPUNIT_TEST(modMixFloatInt);
        CPPUNIT_TEST(modMixIntFloat);

    // BinBoolTest.cpp
        CPPUNIT_TEST(binEqualTrueTrue);
        CPPUNIT_TEST(binEqualTrueFalse);
        CPPUNIT_TEST(binEqualFalseTrue);
        CPPUNIT_TEST(binEqualFalseFalse);
        CPPUNIT_TEST(binNotEqualTrueTrue);
        CPPUNIT_TEST(binNotEqualTrueFalse);
        CPPUNIT_TEST(binNotEqualFalseTrue);
        CPPUNIT_TEST(binNotEqualFalseFalse);
        CPPUNIT_TEST(binAndTrueTrue);
        CPPUNIT_TEST(binAndTrueFalse);
        CPPUNIT_TEST(binAndFalseTrue);
        CPPUNIT_TEST(binAndFalseFalse);
        CPPUNIT_TEST(binOrTrueTrue);
        CPPUNIT_TEST(binOrTrueFalse);
        CPPUNIT_TEST(binOrFalseTrue);
        CPPUNIT_TEST(binOrFalseFalse);
        CPPUNIT_TEST(binGrEqTrueTrue);
        CPPUNIT_TEST(binGrEqTrueFalse);
        CPPUNIT_TEST(binGrEqFalseTrue);
        CPPUNIT_TEST(binGrEqFalseFalse);
        CPPUNIT_TEST(binLeEqTrueTrue);
        CPPUNIT_TEST(binLeEqTrueFalse);
        CPPUNIT_TEST(binLeEqFalseTrue);
        CPPUNIT_TEST(binLeEqFalseFalse);
        CPPUNIT_TEST(binGreatTrueTrue);
        CPPUNIT_TEST(binGreatTrueFalse);
        CPPUNIT_TEST(binGreatFalseTrue);
        CPPUNIT_TEST(binGreatFalseFalse);
        CPPUNIT_TEST(binLessTrueTrue);
        CPPUNIT_TEST(binLessTrueFalse);
        CPPUNIT_TEST(binLessFalseTrue);
        CPPUNIT_TEST(binLessFalseFalse);

    // BinValTest.cpp
        CPPUNIT_TEST(binEqualIntInt);
        CPPUNIT_TEST(binNotEqualIntInt);
        CPPUNIT_TEST(binAndIntInt);
        CPPUNIT_TEST(binOrIntInt);
        CPPUNIT_TEST(binGrEqIntInt);
        CPPUNIT_TEST(binLeEqIntInt);
        CPPUNIT_TEST(binGreatIntInt);
        CPPUNIT_TEST(binLessIntInt);
        CPPUNIT_TEST(binEqualFloatFloat);
        CPPUNIT_TEST(binNotEqualFloatFloat);
        CPPUNIT_TEST(binAndFloatFloat);
        CPPUNIT_TEST(binOrFloatFloat);
        CPPUNIT_TEST(binGrEqFloatFloat);
        CPPUNIT_TEST(binLeEqFloatFloat);
        CPPUNIT_TEST(binGreatFloatFloat);
        CPPUNIT_TEST(binLessFloatFloat);

    // SigTest.cpp
        CPPUNIT_TEST(sigLengthZeroInt);
        CPPUNIT_TEST(sigLengthOneInt);
        CPPUNIT_TEST(sigLengthTwoInt);
        CPPUNIT_TEST(sigLengthThreeInt);
        CPPUNIT_TEST(sigLengthFourInt);
        CPPUNIT_TEST(sigLengthFiveInt);
        CPPUNIT_TEST(sigLengthSixInt);
        CPPUNIT_TEST(sigLengthSevenInt);
        CPPUNIT_TEST(sigLengthEightInt);
        CPPUNIT_TEST(sigLengthNineInt);
        CPPUNIT_TEST(sigLengthTenInt);
        CPPUNIT_TEST(patLengthSameInt);
        CPPUNIT_TEST(patLengthLessInt);
        CPPUNIT_TEST(patLengthMoreInt);

    // CasTest.cpp
        CPPUNIT_TEST(casCountNoInt);
        CPPUNIT_TEST(casCountZeroInt);
        CPPUNIT_TEST(casCountOneInt);
        CPPUNIT_TEST(casCountTwoInt);
        CPPUNIT_TEST(casCountThreeInt);
        CPPUNIT_TEST(casCountFourInt);
        CPPUNIT_TEST(casCountFiveInt);
        CPPUNIT_TEST(casCountSixInt);
        CPPUNIT_TEST(casCountSevenInt);
        CPPUNIT_TEST(casCountEightInt);
        CPPUNIT_TEST(casCountNineInt);
        CPPUNIT_TEST(casCountTenInt);

    // ListTest.cpp
        CPPUNIT_TEST(listCasEmpty);
        CPPUNIT_TEST(listAppendEmpty);
        CPPUNIT_TEST(listInt);
        CPPUNIT_TEST(listFloat);
        CPPUNIT_TEST(listBool);
        CPPUNIT_TEST(listChar);
        CPPUNIT_TEST(listString);
        CPPUNIT_TEST(listListInt);
        CPPUNIT_TEST(listListFloat);
        CPPUNIT_TEST(listListBool);
        CPPUNIT_TEST(listListChar);
        CPPUNIT_TEST(listListString);
        CPPUNIT_TEST(listListListInt);
        CPPUNIT_TEST(listAppendInt);
        CPPUNIT_TEST(listAppendFloat);
        CPPUNIT_TEST(listAppendBool);
        CPPUNIT_TEST(listAppendChar);
        CPPUNIT_TEST(listAppendString);
        CPPUNIT_TEST(listLongInt);
        CPPUNIT_TEST(listFloatInInt);
        CPPUNIT_TEST(listBoolInInt);
        CPPUNIT_TEST(listCharInInt);
        CPPUNIT_TEST(listStringInInt);
        CPPUNIT_TEST(listIntInFloat);
        CPPUNIT_TEST(listBoolInFloat);
        CPPUNIT_TEST(listCharInFloat);
        CPPUNIT_TEST(listStringInFloat);
        CPPUNIT_TEST(listIntInBool);
        CPPUNIT_TEST(listFloatInBool);
        CPPUNIT_TEST(listCharInBool);
        CPPUNIT_TEST(listStringInBool);
        CPPUNIT_TEST(listIntInChar);
        CPPUNIT_TEST(listFloatInChar);
        CPPUNIT_TEST(listBoolInChar);
        CPPUNIT_TEST(listStringInChar);
        CPPUNIT_TEST(listIntInString);
        CPPUNIT_TEST(listFloatInString);
        CPPUNIT_TEST(listBoolInString);
        CPPUNIT_TEST(listCharInString);
        CPPUNIT_TEST(listMixTypes);
        CPPUNIT_TEST(listNestedDifLengthInt);
        CPPUNIT_TEST(listTupleInt);
        CPPUNIT_TEST(listTupleFloat);
        CPPUNIT_TEST(listTupleBool);
        CPPUNIT_TEST(listTupleChar);
        CPPUNIT_TEST(listTupleString);

    // TupleTest.cpp
        CPPUNIT_TEST(tupleEmpty);
        CPPUNIT_TEST(tupleEmptyInt);
        CPPUNIT_TEST(tupleEmptyFloat);
        CPPUNIT_TEST(tupleEmptyBool);
        CPPUNIT_TEST(tupleEmptyChar);
        CPPUNIT_TEST(tupleEmptyString);
        CPPUNIT_TEST(tupleOneInt);
        CPPUNIT_TEST(tupleOneFloat);
        CPPUNIT_TEST(tupleOneBool);
        CPPUNIT_TEST(tupleOneChar);
        CPPUNIT_TEST(tupleOneString);
        CPPUNIT_TEST(tupleTwoInt);
        CPPUNIT_TEST(tupleTwoFloat);
        CPPUNIT_TEST(tupleTwoBool);
        CPPUNIT_TEST(tupleTwoChar);
        CPPUNIT_TEST(tupleTwoString);
        CPPUNIT_TEST(tupleThreeInt);
        CPPUNIT_TEST(tupleThreeFloat);
        CPPUNIT_TEST(tupleThreeBool);
        CPPUNIT_TEST(tupleThreeChar);
        CPPUNIT_TEST(tupleThreeString);
        CPPUNIT_TEST(tupleMixed);
        CPPUNIT_TEST(tupleListInt);
        CPPUNIT_TEST(tupleListFloat);
        CPPUNIT_TEST(tupleListBool);
        CPPUNIT_TEST(tupleListChar);
        CPPUNIT_TEST(tupleListString);
        CPPUNIT_TEST(tupleNested);
        CPPUNIT_TEST(tupleTwoNested);
        CPPUNIT_TEST(tupleSuperNested);
        CPPUNIT_TEST(tupleWrongType);

    // FuncTest.cpp
        CPPUNIT_TEST(funcRetInt);
        CPPUNIT_TEST(funcRetFloat);
        CPPUNIT_TEST(funcRetBool);
        CPPUNIT_TEST(funcRetChar);
        CPPUNIT_TEST(funcRetString);
        CPPUNIT_TEST(funcIntRetInt);
        CPPUNIT_TEST(funcFloatRetFloat);
        CPPUNIT_TEST(funcBoolRetBool);
        CPPUNIT_TEST(funcCharRetChar);
        CPPUNIT_TEST(funcStringRetString);
        CPPUNIT_TEST(funcIntIntRetInt);
        CPPUNIT_TEST(funcFloatFloatRetFloat);
        CPPUNIT_TEST(funcBoolBoolRetBool);
        CPPUNIT_TEST(funcCharCharRetChar);
        CPPUNIT_TEST(funcStringStringRetString);
        CPPUNIT_TEST(funcListIntRetListInt);
        CPPUNIT_TEST(funcListFloatRetListFloat);
        CPPUNIT_TEST(funcListBoolRetListBool);
        CPPUNIT_TEST(funcListCharRetListChar);
        CPPUNIT_TEST(funcListStringRetListString);
        CPPUNIT_TEST(funcTupleIntIntRetTupleIntInt);
        CPPUNIT_TEST(funcTupleFloatFloatRetTupleFloatFloat);
        CPPUNIT_TEST(funcTupleBoolBoolRetTupleBoolBool);
        CPPUNIT_TEST(funcTupleCharCharRetTupleCharChar);
        CPPUNIT_TEST(funcTupleStringStringRetTupleStringString);
        CPPUNIT_TEST(funcIntToIntRetInt);
        CPPUNIT_TEST(funcFloatToFloatRetFloat);
        CPPUNIT_TEST(funcBoolToBoolRetBool);
        CPPUNIT_TEST(funcCharToCharRetChar);
        CPPUNIT_TEST(funcStringToStringRetString);
        CPPUNIT_TEST(funcListIntToListIntRetListInt);
        CPPUNIT_TEST(funcTupleIntIntToTupleIntIntRetInt);
        CPPUNIT_TEST(funcIntToIntRetToIntRetToInt);
        CPPUNIT_TEST(funcFuncOverloadLength);
        CPPUNIT_TEST(funcFuncOverloadType);
        CPPUNIT_TEST(funcRetWrong);
        CPPUNIT_TEST(funcParamCount);
        CPPUNIT_TEST(funcParamCountLess);
        CPPUNIT_TEST(funcParamCountMore);

    // KeywordTest.cpp
        CPPUNIT_TEST(keywordInt);
        CPPUNIT_TEST(keywordFloat);
        CPPUNIT_TEST(keywordBool);
        CPPUNIT_TEST(keywordChar);
        CPPUNIT_TEST(keywordString);
        CPPUNIT_TEST(keywordDef);
        CPPUNIT_TEST(keywordMain);
        CPPUNIT_TEST(comment);

    // ScopeTest.cpp
        CPPUNIT_TEST(scopeNormal);
        CPPUNIT_TEST(scopeSamePat);
        CPPUNIT_TEST(scopeSameFunc);
        CPPUNIT_TEST(scopeUndeclId);

    // DotTest.cpp
        CPPUNIT_TEST(dotNormal);
        CPPUNIT_TEST(dotNumDot);
        CPPUNIT_TEST(dotDotNum);
    CPPUNIT_TEST_SUITE_END();
public:
    compiler::Backend backend;
    void setUp();
    void tearDown();
protected:
    std::string compMsgFail = "Compilation Failed Unexpectedly!";       // Displayed when a success is expected
    std::string compMsgSucc = "Compilation Succeeded Unexpectedly!";    // Displayed when a failure is expected
    std::string execMsg = "Execution Failed";                           // If it can execute, it should succeed

// AddTest.cpp
    void addIntPosPos();
    void addIntZeroZero();
    void addIntPosNeg();
    void addIntNegPos();
    void addIntNegNeg();
    void addFloatPosPos();
    void addFloatZeroZero();
    void addFloatPosNeg();
    void addFloatNegPos();
    void addFloatNegNeg();
    void addBoolTrueTrue();
    void addBoolTrueFalse();
    void addBoolFalseTrue();
    void addBoolFalseFalse();
    void addCharStrStr();
    void addCharStrCha();
    void addCharChaStr();
    void addCharChaCha();
    void addMixFloatInt();
    void addMixIntFloat();

// SubTest.cpp
    void subIntPosPos();
    void subIntZeroZero();
    void subIntPosNeg();
    void subIntNegPos();
    void subIntNegNeg();
    void subFloatPosPos();
    void subFloatZeroZero();
    void subFloatPosNeg();
    void subFloatNegPos();
    void subFloatNegNeg();
    void subBoolTrueTrue();
    void subBoolTrueFalse();
    void subBoolFalseTrue();
    void subBoolFalseFalse();
    void subCharStrStr();
    void subCharStrCha();
    void subCharChaStr();
    void subCharChaCha();
    void subMixFloatInt();
    void subMixIntFloat();
    void subBoolNegative();

// MulTest.cpp
    void mulIntPosPos();
    void mulIntZeroZero();
    void mulIntPosNeg();
    void mulIntNegPos();
    void mulIntNegNeg();
    void mulFloatPosPos();
    void mulFloatZeroZero();
    void mulFloatPosNeg();
    void mulFloatNegPos();
    void mulFloatNegNeg();
    void mulBoolTrueTrue();
    void mulBoolTrueFalse();
    void mulBoolFalseTrue();
    void mulBoolFalseFalse();
    void mulCharStrStr();
    void mulCharStrCha();
    void mulCharChaStr();
    void mulCharChaCha();
    void mulMixFloatInt();
    void mulMixIntFloat();

// DivTest.cpp
    void divIntPosPos();
    void divIntZeroZero();
    void divIntPosNeg();
    void divIntNegPos();
    void divIntNegNeg();
    void divFloatPosPos();
    void divFloatZeroZero();
    void divFloatPosNeg();
    void divFloatNegPos();
    void divFloatNegNeg();
    void divBoolTrueTrue();
    void divBoolTrueFalse();
    void divBoolFalseTrue();
    void divBoolFalseFalse();
    void divCharStrStr();
    void divCharStrCha();
    void divCharChaStr();
    void divCharChaCha();
    void divMixFloatInt();
    void divMixIntFloat();

// ModTest.cpp
    void modIntPosPos();
    void modIntZeroZero();
    void modIntPosNeg();
    void modIntNegPos();
    void modIntNegNeg();
    void modFloatPosPos();
    void modFloatZeroZero();
    void modFloatPosNeg();
    void modFloatNegPos();
    void modFloatNegNeg();
    void modBoolTrueTrue();
    void modBoolTrueFalse();
    void modBoolFalseTrue();
    void modBoolFalseFalse();
    void modCharStrStr();
    void modCharStrCha();
    void modCharChaStr();
    void modCharChaCha();
    void modMixFloatInt();
    void modMixIntFloat();

// BinBoolTest.cpp
    void binEqualTrueTrue();
    void binEqualTrueFalse();
    void binEqualFalseTrue();
    void binEqualFalseFalse();
    void binNotEqualTrueTrue();
    void binNotEqualTrueFalse();
    void binNotEqualFalseTrue();
    void binNotEqualFalseFalse();
    void binAndTrueTrue();
    void binAndTrueFalse();
    void binAndFalseTrue();
    void binAndFalseFalse();
    void binOrTrueTrue();
    void binOrTrueFalse();
    void binOrFalseTrue();
    void binOrFalseFalse();
    void binGrEqTrueTrue();
    void binGrEqTrueFalse();
    void binGrEqFalseTrue();
    void binGrEqFalseFalse();
    void binLeEqTrueTrue();
    void binLeEqTrueFalse();
    void binLeEqFalseTrue();
    void binLeEqFalseFalse();
    void binGreatTrueTrue();
    void binGreatTrueFalse();
    void binGreatFalseTrue();
    void binGreatFalseFalse();
    void binLessTrueTrue();
    void binLessTrueFalse();
    void binLessFalseTrue();
    void binLessFalseFalse();

// BinValTest.cpp
    void binEqualIntInt();
    void binNotEqualIntInt();
    void binAndIntInt();
    void binOrIntInt();
    void binGrEqIntInt();
    void binLeEqIntInt();
    void binGreatIntInt();
    void binLessIntInt();
    void binEqualFloatFloat();
    void binNotEqualFloatFloat();
    void binAndFloatFloat();
    void binOrFloatFloat();
    void binGrEqFloatFloat();
    void binLeEqFloatFloat();
    void binGreatFloatFloat();
    void binLessFloatFloat();

// SigTest.cpp
    void sigLengthZeroInt();
    void sigLengthOneInt();
    void sigLengthTwoInt();
    void sigLengthThreeInt();
    void sigLengthFourInt();
    void sigLengthFiveInt();
    void sigLengthSixInt();
    void sigLengthSevenInt();
    void sigLengthEightInt();
    void sigLengthNineInt();
    void sigLengthTenInt();
    void patLengthSameInt();
    void patLengthLessInt();
    void patLengthMoreInt();

// CasTest.cpp
    void casCountNoInt();
    void casCountZeroInt();
    void casCountOneInt();
    void casCountTwoInt();
    void casCountThreeInt();
    void casCountFourInt();
    void casCountFiveInt();
    void casCountSixInt();
    void casCountSevenInt();
    void casCountEightInt();
    void casCountNineInt();
    void casCountTenInt();

// ListTest.cpp
    void listCasEmpty();
    void listAppendEmpty();
    void listInt();
    void listFloat();
    void listBool();
    void listChar();
    void listString();
    void listListInt();
    void listListFloat();
    void listListBool();
    void listListChar();
    void listListString();
    void listListListInt();
    void listAppendInt();
    void listAppendFloat();
    void listAppendBool();
    void listAppendChar();
    void listAppendString();
    void listLongInt();
    void listFloatInInt();
    void listBoolInInt();
    void listCharInInt();
    void listStringInInt();
    void listIntInFloat();
    void listBoolInFloat();
    void listCharInFloat();
    void listStringInFloat();
    void listIntInBool();
    void listFloatInBool();
    void listCharInBool();
    void listStringInBool();
    void listIntInChar();
    void listFloatInChar();
    void listBoolInChar();
    void listStringInChar();
    void listIntInString();
    void listFloatInString();
    void listBoolInString();
    void listCharInString();
    void listMixTypes();
    void listNestedDifLengthInt();
    void listTupleInt();
    void listTupleFloat();
    void listTupleBool();
    void listTupleChar();
    void listTupleString();

// TupleTest.cpp
    void tupleEmpty();
    void tupleEmptyInt();
    void tupleEmptyFloat();
    void tupleEmptyBool();
    void tupleEmptyChar();
    void tupleEmptyString();
    void tupleOneInt();
    void tupleOneFloat();
    void tupleOneBool();
    void tupleOneChar();
    void tupleOneString();
    void tupleTwoInt();
    void tupleTwoFloat();
    void tupleTwoBool();
    void tupleTwoChar();
    void tupleTwoString();
    void tupleThreeInt();
    void tupleThreeFloat();
    void tupleThreeBool();
    void tupleThreeChar();
    void tupleThreeString();
    void tupleMixed();
    void tupleListInt();
    void tupleListFloat();
    void tupleListBool();
    void tupleListChar();
    void tupleListString();
    void tupleNested();
    void tupleTwoNested();
    void tupleSuperNested();
    void tupleWrongType();

// FuncTest.cpp
    void funcRetInt();
    void funcRetFloat();
    void funcRetBool();
    void funcRetChar();
    void funcRetString();
    void funcIntRetInt();
    void funcFloatRetFloat();
    void funcBoolRetBool();
    void funcCharRetChar();
    void funcStringRetString();
    void funcIntIntRetInt();
    void funcFloatFloatRetFloat();
    void funcBoolBoolRetBool();
    void funcCharCharRetChar();
    void funcStringStringRetString();
    void funcListIntRetListInt();
    void funcListFloatRetListFloat();
    void funcListBoolRetListBool();
    void funcListCharRetListChar();
    void funcListStringRetListString();
    void funcTupleIntIntRetTupleIntInt();
    void funcTupleFloatFloatRetTupleFloatFloat();
    void funcTupleBoolBoolRetTupleBoolBool();
    void funcTupleCharCharRetTupleCharChar();
    void funcTupleStringStringRetTupleStringString();
    void funcIntToIntRetInt();
    void funcFloatToFloatRetFloat();
    void funcBoolToBoolRetBool();
    void funcCharToCharRetChar();
    void funcStringToStringRetString();
    void funcListIntToListIntRetListInt();
    void funcTupleIntIntToTupleIntIntRetInt();
    void funcIntToIntRetToIntRetToInt();
    void funcFuncOverloadLength();
    void funcFuncOverloadType();
    void funcRetWrong();
    void funcParamCount();
    void funcParamCountLess();
    void funcParamCountMore();

// KeywordTest.cpp
    void keywordInt();
    void keywordFloat();
    void keywordBool();
    void keywordChar();
    void keywordString();
    void keywordDef();
    void keywordMain();
    void comment();

// Scope.cpp
    void scopeNormal();
    void scopeSamePat();
    void scopeSameFunc();
    void scopeUndeclId();

    // Dot.cpp
    void dotNormal();
    void dotNumDot();
    void dotDotNum();

    // Test.cpp
    bool compileChecker(std::string name);
    bool executeChecker(std::string args, std::string expectedOutput);
    bool executeChecker(std::string expectedOutput);
    bool executeCPP(std::string args, std::string expectedOutput);
    bool executeLLVM(std::string args, std::string expectedOutput);
    bool checkIfFileExists(string file);
};