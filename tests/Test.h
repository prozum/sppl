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
        // Addition - Integer
        CPPUNIT_TEST(addIntPosPos);
        CPPUNIT_TEST(addIntZeroZero);
        CPPUNIT_TEST(addIntNegPos);
        CPPUNIT_TEST(addIntPosNeg);
        CPPUNIT_TEST(addIntNegNeg);

        // Addition - Float
        CPPUNIT_TEST(addFloatPosPos);
        CPPUNIT_TEST(addFloatZeroZero);
        CPPUNIT_TEST(addFloatPosNeg);
        CPPUNIT_TEST(addFloatNegPos);
        CPPUNIT_TEST(addFloatNegNeg);
        // Addition - Boolean
        CPPUNIT_TEST(addBoolTrueTrue);
        CPPUNIT_TEST(addBoolTrueFalse);
        CPPUNIT_TEST(addBoolFalseTrue);
        CPPUNIT_TEST(addBoolFalseFalse);
        // Addition - Character
        CPPUNIT_TEST(addCharStrStr);
        CPPUNIT_TEST(addCharStrChar);
        CPPUNIT_TEST(addCharCharStr);
        CPPUNIT_TEST(addCharCharChar);
        // Addition - Mix
        CPPUNIT_TEST(addMixFloatInt);
        CPPUNIT_TEST(addMixIntFloat);
        // Subtraction - Integer
        CPPUNIT_TEST(subIntPosPos);
        CPPUNIT_TEST(subIntZeroZero);
        CPPUNIT_TEST(subIntNegPos);
        CPPUNIT_TEST(subIntPosNeg);
        CPPUNIT_TEST(subIntNegNeg);
        // Subtraction - Float
        CPPUNIT_TEST(subFloatPosPos);
        CPPUNIT_TEST(subFloatZeroZero);
        CPPUNIT_TEST(subFloatPosNeg);
        CPPUNIT_TEST(subFloatNegPos);
        CPPUNIT_TEST(subFloatNegNeg);
        // Subtraction - Boolean
        CPPUNIT_TEST(subBoolTrueTrue);
        CPPUNIT_TEST(subBoolTrueFalse);
        CPPUNIT_TEST(subBoolFalseTrue);
        CPPUNIT_TEST(subBoolFalseFalse);
        // Subtraction - Character
        CPPUNIT_TEST(subCharStrStr);
        CPPUNIT_TEST(subCharStrChar);
        CPPUNIT_TEST(subCharCharStr);
        CPPUNIT_TEST(subCharCharChar);
        // Subtraction - Mix
        CPPUNIT_TEST(subMixFloatInt);
        CPPUNIT_TEST(subMixIntFloat);
        // Multiplication - Integer
        CPPUNIT_TEST(mulIntPosPos);
        CPPUNIT_TEST(mulIntZeroZero);
        CPPUNIT_TEST(mulIntNegPos);
        CPPUNIT_TEST(mulIntPosNeg);
        CPPUNIT_TEST(mulIntNegNeg);
        // Multiplication - Float
        CPPUNIT_TEST(mulFloatPosPos);
        CPPUNIT_TEST(mulFloatZeroZero);
        CPPUNIT_TEST(mulFloatPosNeg);
        CPPUNIT_TEST(mulFloatNegPos);
        CPPUNIT_TEST(mulFloatNegNeg);
        // Multiplication - Boolean
        CPPUNIT_TEST(mulBoolTrueTrue);
        CPPUNIT_TEST(mulBoolTrueFalse);
        CPPUNIT_TEST(mulBoolFalseTrue);
        CPPUNIT_TEST(mulBoolFalseFalse);
        // Multiplication - Character
        CPPUNIT_TEST(mulCharStrStr);
        CPPUNIT_TEST(mulCharStrChar);
        CPPUNIT_TEST(mulCharCharStr);
        CPPUNIT_TEST(mulCharCharChar);
        // Multiplication - Mix
        CPPUNIT_TEST(mulMixFloatInt);
        CPPUNIT_TEST(mulMixIntFloat);
        // Division - Integer
        CPPUNIT_TEST(divIntPosPos);
        CPPUNIT_TEST(divIntZeroZero);
        CPPUNIT_TEST(divIntNegPos);
        CPPUNIT_TEST(divIntPosNeg);
        CPPUNIT_TEST(divIntNegNeg);
        // Division - Float
        CPPUNIT_TEST(divFloatPosPos);
        CPPUNIT_TEST(divFloatZeroZero);
        CPPUNIT_TEST(divFloatPosNeg);
        CPPUNIT_TEST(divFloatNegPos);
        CPPUNIT_TEST(divFloatNegNeg);
        // Division - Boolean
        CPPUNIT_TEST(divBoolTrueTrue);
        CPPUNIT_TEST(divBoolTrueFalse);
        CPPUNIT_TEST(divBoolFalseTrue);
        CPPUNIT_TEST(divBoolFalseFalse);
        // Division - Character
        CPPUNIT_TEST(divCharStrStr);
        CPPUNIT_TEST(divCharStrChar);
        CPPUNIT_TEST(divCharCharStr);
        CPPUNIT_TEST(divCharCharChar);
        // Division - Mix
        CPPUNIT_TEST(divMixFloatInt);
        CPPUNIT_TEST(divMixIntFloat);
        // Modulo - Integer
        CPPUNIT_TEST(modIntPosPos);
        CPPUNIT_TEST(modIntZeroZero);
        CPPUNIT_TEST(modIntNegPos);
        CPPUNIT_TEST(modIntPosNeg);
        CPPUNIT_TEST(modIntNegNeg);
        // Modulo - Float
        CPPUNIT_TEST(modFloatPosPos);
        CPPUNIT_TEST(modFloatZeroZero);
        CPPUNIT_TEST(modFloatPosNeg);
        CPPUNIT_TEST(modFloatNegPos);
        CPPUNIT_TEST(modFloatNegNeg);
        // Modulo - Boolean
        CPPUNIT_TEST(modBoolTrueTrue);
        CPPUNIT_TEST(modBoolTrueFalse);
        CPPUNIT_TEST(modBoolFalseTrue);
        CPPUNIT_TEST(modBoolFalseFalse);
        // Modulo - Character
        CPPUNIT_TEST(modCharStrStr);
        CPPUNIT_TEST(modCharStrChar);
        CPPUNIT_TEST(modCharCharStr);
        CPPUNIT_TEST(modCharCharChar);
        // Modulo - Mix
        CPPUNIT_TEST(modMixFloatInt);
        CPPUNIT_TEST(modMixIntFloat);
        // Binary/Bool - Equal
        CPPUNIT_TEST(binEqualTrueTrue);
        CPPUNIT_TEST(binEqualTrueFalse);
        CPPUNIT_TEST(binEqualFalseTrue);
        CPPUNIT_TEST(binEqualFalseFalse);
        // Binary/Bool - NotEqual
        CPPUNIT_TEST(binNotEqualTrueTrue);
        CPPUNIT_TEST(binNotEqualTrueFalse);
        CPPUNIT_TEST(binNotEqualFalseTrue);
        CPPUNIT_TEST(binNotEqualFalseFalse);
        // Binary/Bool - And
        CPPUNIT_TEST(binAndTrueTrue);
        CPPUNIT_TEST(binAndTrueFalse);
        CPPUNIT_TEST(binAndFalseTrue);
        CPPUNIT_TEST(binAndFalseFalse);
        // Binary/Bool - Or
        CPPUNIT_TEST(binOrTrueTrue);
        CPPUNIT_TEST(binOrTrueFalse);
        CPPUNIT_TEST(binOrFalseTrue);
        CPPUNIT_TEST(binOrFalseFalse);
        // Binary/Bool - GrEq
        CPPUNIT_TEST(binGrEqTrueTrue);
        CPPUNIT_TEST(binGrEqTrueFalse);
        CPPUNIT_TEST(binGrEqFalseTrue);
        CPPUNIT_TEST(binGrEqFalseFalse);
        // Binary/Bool - LeEq
        CPPUNIT_TEST(binLeEqTrueTrue);
        CPPUNIT_TEST(binLeEqTrueFalse);
        CPPUNIT_TEST(binLeEqFalseTrue);
        CPPUNIT_TEST(binLeEqFalseFalse);
        // Binary/Bool - Great
        CPPUNIT_TEST(binGreatTrueTrue);
        CPPUNIT_TEST(binGreatTrueFalse);
        CPPUNIT_TEST(binGreatFalseTrue);
        CPPUNIT_TEST(binGreatFalseFalse);
        // Binary/Bool - Less
        CPPUNIT_TEST(binLessTrueTrue);
        CPPUNIT_TEST(binLessTrueFalse);
        CPPUNIT_TEST(binLessFalseTrue);
        CPPUNIT_TEST(binLessFalseFalse);
        // Binary/Int
        CPPUNIT_TEST(binEqualInt);
        CPPUNIT_TEST(binNotEqualInt);
        CPPUNIT_TEST(binAndInt);
        CPPUNIT_TEST(binOrInt);
        CPPUNIT_TEST(binGrEqInt);
        CPPUNIT_TEST(binLeEqInt);
        CPPUNIT_TEST(binLessInt);
        CPPUNIT_TEST(binGreatInt);
        // Binary/Float
        CPPUNIT_TEST(binEqualFloat);
        CPPUNIT_TEST(binNotEqualFloat);
        CPPUNIT_TEST(binAndFloat);
        CPPUNIT_TEST(binOrFloat);
        CPPUNIT_TEST(binGrEqFloat);
        CPPUNIT_TEST(binLeEqFloat);
        CPPUNIT_TEST(binLessFloat);
        CPPUNIT_TEST(binGreatFloat);
        // Signature - Length
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
        // Pattern - Length
        CPPUNIT_TEST(patLengthSameInt);
        CPPUNIT_TEST(patLengthMoreInt);
        CPPUNIT_TEST(patLengthLessInt);
        // Case - Count
        CPPUNIT_TEST(casOneCaseInt);
        CPPUNIT_TEST(casTwoCaseInt);
        CPPUNIT_TEST(casThreeCaseInt);
        CPPUNIT_TEST(casFourCaseInt);
        CPPUNIT_TEST(casFiveCaseInt);
        CPPUNIT_TEST(casSixCaseInt);
        CPPUNIT_TEST(casSevenCaseInt);
        CPPUNIT_TEST(casEightCaseInt);
        CPPUNIT_TEST(casNineCaseInt);
        CPPUNIT_TEST(casTenCaseInt);
        // List
        CPPUNIT_TEST(listCasEmpty);
        CPPUNIT_TEST(listAppendEmpty);
        // List - Type
        CPPUNIT_TEST(listInt);
        CPPUNIT_TEST(listFloat);
        CPPUNIT_TEST(listBool);
        CPPUNIT_TEST(listChar);
        CPPUNIT_TEST(listString);
        CPPUNIT_TEST(listLongInt);
        // List In List
        CPPUNIT_TEST(listListInt);
        CPPUNIT_TEST(listListFloat);
        CPPUNIT_TEST(listListBool);
        CPPUNIT_TEST(listListChar);
        CPPUNIT_TEST(listListString);
        CPPUNIT_TEST(listListListInt);
        CPPUNIT_TEST(listNestedDifLengthInt);
        // List Append
        CPPUNIT_TEST(listAppendInt);
        CPPUNIT_TEST(listAppendFloat);
        CPPUNIT_TEST(listAppendBool);
        CPPUNIT_TEST(listAppendChar);
        CPPUNIT_TEST(listAppendString);
        // List Incorrect Type - Int
        CPPUNIT_TEST(listFloatInInt);
        CPPUNIT_TEST(listBoolInInt);
        CPPUNIT_TEST(listCharInInt);
        CPPUNIT_TEST(listStringInInt);
        // List Incorrect Type - Float
        CPPUNIT_TEST(listIntInFloat);
        CPPUNIT_TEST(listBoolInFloat);
        CPPUNIT_TEST(listCharInFloat);
        CPPUNIT_TEST(listStringInFloat);
        // List Incorrect Type - Bool
        CPPUNIT_TEST(listIntInBool);
        CPPUNIT_TEST(listFloatInBool);
        CPPUNIT_TEST(listCharInBool);
        CPPUNIT_TEST(listStringInBool);
        // List Incorrect Type - Char
        CPPUNIT_TEST(listIntInChar);
        CPPUNIT_TEST(listFloatInChar);
        CPPUNIT_TEST(listBoolInChar);
        CPPUNIT_TEST(listStringInChar);
        // List Incorrect Type - String
        CPPUNIT_TEST(listIntInString);
        CPPUNIT_TEST(listFloatInString);
        CPPUNIT_TEST(listBoolInString);
        CPPUNIT_TEST(listCharInString);
        // List - Mix
        CPPUNIT_TEST(listMixTypes);
        // List - Tuple
        CPPUNIT_TEST(listTupleInt);
        CPPUNIT_TEST(listTupleFloat);
        CPPUNIT_TEST(listTupleBool);
        CPPUNIT_TEST(listTupleChar);
        CPPUNIT_TEST(listTupleString);
        // Tuple - Empty
        CPPUNIT_TEST(tupleEmpty);
        CPPUNIT_TEST(tupleEmptyInt);
        CPPUNIT_TEST(tupleEmptyFloat);
        CPPUNIT_TEST(tupleEmptyBool);
        CPPUNIT_TEST(tupleEmptyChar);
        CPPUNIT_TEST(tupleEmptyString);
        // Tuple - One
        CPPUNIT_TEST(tupleOneInt);
        CPPUNIT_TEST(tupleOneFloat);
        CPPUNIT_TEST(tupleOneBool);
        CPPUNIT_TEST(tupleOneChar);
        CPPUNIT_TEST(tupleOneString);
        // Tuple - Two
        CPPUNIT_TEST(tupleTwoInt);
        CPPUNIT_TEST(tupleTwoFloat);
        CPPUNIT_TEST(tupleTwoBool);
        CPPUNIT_TEST(tupleTwoChar);
        CPPUNIT_TEST(tupleTwoString);
        // Tuple - Three
        CPPUNIT_TEST(tupleThreeInt);
        CPPUNIT_TEST(tupleThreeFloat);
        CPPUNIT_TEST(tupleThreeBool);
        CPPUNIT_TEST(tupleThreeChar);
        CPPUNIT_TEST(tupleThreeString);
        // Tuple - Mix
        CPPUNIT_TEST(tupleMixed);
        // Tuple - List
        CPPUNIT_TEST(tupleListInt);
        CPPUNIT_TEST(tupleListFloat);
        CPPUNIT_TEST(tupleListBool);
        CPPUNIT_TEST(tupleListChar);
        CPPUNIT_TEST(tupleListString);
        // Tuple - Nested
        CPPUNIT_TEST(tupleNested);
        CPPUNIT_TEST(tupleTwoNested);
        CPPUNIT_TEST(tupleSuperNested);
        // Tuple - Wrong
        CPPUNIT_TEST(tupleWrongType);
        // keyword
        CPPUNIT_TEST(keywordInt);
        CPPUNIT_TEST(keywordFloat);
        CPPUNIT_TEST(keywordChar);
        CPPUNIT_TEST(keywordString);
        CPPUNIT_TEST(keywordBool);
        CPPUNIT_TEST(keywordTrue);
        CPPUNIT_TEST(keywordFalse);
        CPPUNIT_TEST(keywordMain);
        CPPUNIT_TEST(keywordDef);
        // Functions - Ret Empty
        CPPUNIT_TEST(funcRetInt);
        CPPUNIT_TEST(funcRetFloat);
        CPPUNIT_TEST(funcRetBool);
        CPPUNIT_TEST(funcRetChar);
        CPPUNIT_TEST(funcRetString);
        // Functions - One Input
        CPPUNIT_TEST(funcIntRetInt);
        CPPUNIT_TEST(funcFloatRetFloat);
        CPPUNIT_TEST(funcBoolRetBool);
        CPPUNIT_TEST(funcCharRetChar);
        CPPUNIT_TEST(funcStringRetString);
        // Function - MultipleInput
        CPPUNIT_TEST(funcIntIntRetInt);
        CPPUNIT_TEST(funcFloatFloatRetFloat);
        CPPUNIT_TEST(funcBoolBoolRetBool);
        CPPUNIT_TEST(funcCharCharRetChar);
        CPPUNIT_TEST(funcStringStringRetString);
        // Function - Data Structures
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
        // Function - Function As Params
        CPPUNIT_TEST(funcIntToIntRetInt);
        CPPUNIT_TEST(funcFloatToFloatRetFloat);
        CPPUNIT_TEST(funcBoolToBoolRetBool);
        CPPUNIT_TEST(funcCharToCharRetChar);
        CPPUNIT_TEST(funcStringToStringRetString);
        CPPUNIT_TEST(funcListIntToListIntRetListInt);
        CPPUNIT_TEST(funcTupleIntIntToTupleIntIntRetInt);
        CPPUNIT_TEST(funcIntToIntRetToIntRetToInt);
        // Single line comment test
        CPPUNIT_TEST(comment);
    /**/
    CPPUNIT_TEST_SUITE_END();
public:
    compiler::Backend backend = compiler::Backend::CPP ;
    void setUp();
    void tearDown();
protected:
    // Addition
    // Addition - Integer
    void addIntPosPos();
    void addIntZeroZero();
    void addIntPosNeg();
    void addIntNegPos();
    void addIntNegNeg();

    // Addition - Float
    void addFloatPosPos();
    void addFloatZeroZero();
    void addFloatPosNeg();
    void addFloatNegPos();
    void addFloatNegNeg();

    // Addition - Boolean
    void addBoolTrueTrue();
    void addBoolTrueFalse();
    void addBoolFalseTrue();
    void addBoolFalseFalse();

    // Addition - Character
    void addCharStrStr();
    void addCharStrChar();
    void addCharCharStr();
    void addCharCharChar();

    // Addition - Mix
    void addMixIntFloat();
    void addMixFloatInt();

    // Subtraction
    // Subtraction - Integer
    void subIntPosPos();
    void subIntZeroZero();
    void subIntPosNeg();
    void subIntNegPos();
    void subIntNegNeg();

    // Subtraction - Float
    void subFloatPosPos();
    void subFloatZeroZero();
    void subFloatPosNeg();
    void subFloatNegPos();
    void subFloatNegNeg();

    // Subtraction - Boolean
    void subBoolTrueTrue();
    void subBoolTrueFalse();
    void subBoolFalseTrue();
    void subBoolFalseFalse();

    // Subtraction - Character
    void subCharStrStr();
    void subCharStrChar();
    void subCharCharStr();
    void subCharCharChar();

    // Subtraction - Mix
    void subMixIntFloat();
    void subMixFloatInt();

    //Multiplication
    // Multiplication - Integer
    void mulIntPosPos();
    void mulIntZeroZero();
    void mulIntPosNeg();
    void mulIntNegPos();
    void mulIntNegNeg();

    // Multiplication - Float
    void mulFloatPosPos();
    void mulFloatZeroZero();
    void mulFloatPosNeg();
    void mulFloatNegPos();
    void mulFloatNegNeg();

    // Multiplication - Boolean
    void mulBoolTrueTrue();
    void mulBoolTrueFalse();
    void mulBoolFalseTrue();
    void mulBoolFalseFalse();

    // Multiplication - Character
    void mulCharStrStr();
    void mulCharStrChar();
    void mulCharCharStr();
    void mulCharCharChar();

    // Multiplication - Mix
    void mulMixIntFloat();
    void mulMixFloatInt();

    // Division
    // Division - Integer
    void divIntPosPos();
    void divIntZeroZero();
    void divIntPosNeg();
    void divIntNegPos();
    void divIntNegNeg();

    // Division - Float
    void divFloatPosPos();
    void divFloatZeroZero();
    void divFloatPosNeg();
    void divFloatNegPos();
    void divFloatNegNeg();

    // Division - Boolean
    void divBoolTrueTrue();
    void divBoolTrueFalse();
    void divBoolFalseTrue();
    void divBoolFalseFalse();

    // Division - Character
    void divCharStrStr();
    void divCharStrChar();
    void divCharCharStr();
    void divCharCharChar();

    // Division - Mix
    void divMixIntFloat();
    void divMixFloatInt();

    // Modulo
    // Modulo - Integer
    void modIntPosPos();
    void modIntZeroZero();
    void modIntPosNeg();
    void modIntNegPos();
    void modIntNegNeg();

    // Modulo - Float
    void modFloatPosPos();
    void modFloatZeroZero();
    void modFloatPosNeg();
    void modFloatNegPos();
    void modFloatNegNeg();

    // Modulo - Boolean
    void modBoolTrueTrue();
    void modBoolTrueFalse();
    void modBoolFalseTrue();
    void modBoolFalseFalse();

    // Modulo - Character
    void modCharStrStr();
    void modCharStrChar();
    void modCharCharStr();
    void modCharCharChar();

    // Modulo - Mix
    void modMixIntFloat();
    void modMixFloatInt();

    // Binary/Bool
    // Binary/Bool - Equal
    void binEqualTrueTrue();
    void binEqualTrueFalse();
    void binEqualFalseTrue();
    void binEqualFalseFalse();

    // Binary/Bool - NotEqual
    void binNotEqualTrueTrue();
    void binNotEqualTrueFalse();
    void binNotEqualFalseTrue();
    void binNotEqualFalseFalse();

    // Binary/Bool - And
    void binAndTrueTrue();
    void binAndTrueFalse();
    void binAndFalseTrue();
    void binAndFalseFalse();

    // Binary/Bool - Or
    void binOrTrueTrue();
    void binOrTrueFalse();
    void binOrFalseTrue();
    void binOrFalseFalse();

    // Binary/Bool - GrEq
    void binGrEqTrueTrue();
    void binGrEqTrueFalse();
    void binGrEqFalseTrue();
    void binGrEqFalseFalse();

    // Binary/Bool - LeEq
    void binLeEqTrueTrue();
    void binLeEqTrueFalse();
    void binLeEqFalseTrue();
    void binLeEqFalseFalse();

    // Binary/Bool - Less
    void binLessTrueTrue();
    void binLessTrueFalse();
    void binLessFalseTrue();
    void binLessFalseFalse();

    // Binary/Bool - Great
    void binGreatTrueTrue();
    void binGreatTrueFalse();
    void binGreatFalseTrue();
    void binGreatFalseFalse();

    // Binary/Int
    // Binary/Int
    void binEqualInt();
    void binNotEqualInt();
    void binAndInt();
    void binOrInt();
    void binLeEqInt();
    void binGrEqInt();
    void binLessInt();
    void binGreatInt();

    // Binary/Float
    // Binary/Float
    void binEqualFloat();
    void binNotEqualFloat();
    void binAndFloat();
    void binOrFloat();
    void binLeEqFloat();
    void binGrEqFloat();
    void binLessFloat();
    void binGreatFloat();

    // Signature Length
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

    // Pattern Length
    void patLengthSameInt();
    void patLengthMoreInt();
    void patLengthLessInt();

    // Case Count
    void casOneCaseInt();
    void casTwoCaseInt();
    void casThreeCaseInt();
    void casFourCaseInt();
    void casFiveCaseInt();
    void casSixCaseInt();
    void casSevenCaseInt();
    void casEightCaseInt();
    void casNineCaseInt();
    void casTenCaseInt();

    //List
    void listCasEmpty();
    void listAppendEmpty();
    void listInt();
    void listFloat();
    void listBool();
    void listChar();
    void listString();
    void listLongInt();

    // List - Nested
    void listListInt();
    void listListFloat();
    void listListBool();
    void listListChar();
    void listListString();
    void listListListInt();

    // List - Append
    void listAppendInt();
    void listAppendFloat();
    void listAppendBool();
    void listAppendChar();
    void listAppendString();
    
    // List - Wrong Type
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

    // List - Other
    void listNestedDifLengthInt();

    void listTupleInt();
    void listTupleFloat();
    void listTupleBool();
    void listTupleChar();
    void listTupleString();

    // Tuple - Empty
    void tupleEmpty();
    void tupleEmptyInt();
    void tupleEmptyFloat();
    void tupleEmptyBool();
    void tupleEmptyChar();
    void tupleEmptyString();

    // Tuple - One
    void tupleOneInt();
    void tupleOneFloat();
    void tupleOneBool();
    void tupleOneChar();
    void tupleOneString();

    // Tuple - Two
    void tupleTwoInt();
    void tupleTwoFloat();
    void tupleTwoBool();
    void tupleTwoChar();
    void tupleTwoString();

    // Tuple - Three
    void tupleThreeInt();
    void tupleThreeFloat();
    void tupleThreeBool();
    void tupleThreeChar();
    void tupleThreeString();

    // Tuple - List
    void tupleListInt();
    void tupleListFloat();
    void tupleListBool();
    void tupleListChar();
    void tupleListString();

    // Tuple - Other
    void tupleNested();
    void tupleTwoNested();
    void tupleSuperNested();
    void tupleWrongType();
    void tupleMixed();

    // Keywords
    void keywordTrue();
    void keywordFalse();
    void keywordInt();
    void keywordFloat();
    void keywordBool();
    void keywordChar();
    void keywordString();
    void keywordDef();
    void keywordMain();

    // Functions - Ret Empty
    void funcRetInt();
    void funcRetFloat();
    void funcRetBool();
    void funcRetChar();
    void funcRetString();

    // Functions - One Input
    void funcIntRetInt();
    void funcFloatRetFloat();
    void funcBoolRetBool();
    void funcCharRetChar();
    void funcStringRetString();

    // Function - MultipleInput
    void funcIntIntRetInt();
    void funcFloatFloatRetFloat();
    void funcBoolBoolRetBool();
    void funcCharCharRetChar();
    void funcStringStringRetString();

    // Function - Data Structures
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

    // Function - Function As Params
    void funcIntToIntRetInt();
    void funcFloatToFloatRetFloat();
    void funcBoolToBoolRetBool();
    void funcCharToCharRetChar();
    void funcStringToStringRetString();
    void funcListIntToListIntRetListInt();
    void funcTupleIntIntToTupleIntIntRetInt();
    void funcIntToIntRetToIntRetToInt();

    // Single line comment test
    void comment();

    bool compileChecker(shared_ptr<std::stringstream> source);
    bool executeChecker(bool compStatus, std::string args, std::string expectedOutput);
    shared_ptr<std::stringstream> buildSimple(std::string signature,
                                  std::string body);
    shared_ptr<std::stringstream> buildSimple(std::string signature,
                                  std::string pattern,
                                  std::string body);
    shared_ptr<std::stringstream> buildSimple(std::string pattern,
                                  std::string left,
                                  std::string op,
                                  std::string right);
    std::string buildCase(std::string pattern,
                                std::string body);
    shared_ptr<std::stringstream> buildMultiCase(std::vector<string>
                                     signature,
                                     std::string ret,
                                     std::string pattern,
                                     std::string cas);
    shared_ptr<std::stringstream> buildMultiCase(std::string signature,
                                     std::string ret,
                                     std::vector<std::string> pattern,
                                     std::string cas);
    shared_ptr<std::stringstream> buildMultiFunc(std::string mainSig,  std::string mainPat,  std::string mainBody,
                                     std::string funcSig,  std::string funcPat,  std::string funcBody);
    shared_ptr<std::stringstream> buildMultiFunc(std::string mainSig,  std::string mainPat,  std::string mainBody,
                                     std::string func1Sig, std::string func1Pat, std::string func1Body,
                                     std::string func2Sig, std::string func2Pat, std::string func2Body);
    shared_ptr<std::stringstream> buildMultiFunc(std::string mainSig,  std::string mainPat,  std::string mainBody,
                                     std::string func1Sig, std::string func1Pat, std::string func1Body,
                                     std::string func2Sig, std::string func2Pat, std::string func2Body, 
                                     std::string func3Sig, std::string func3Pat, std::string func3Body);
};