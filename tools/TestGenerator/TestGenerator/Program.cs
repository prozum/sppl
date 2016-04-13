using System;
using System.IO;

namespace TestGenerator
{
	class MainClass
	{
		static string compMsg = "Compilation failed";
		static string execMsg = "Execution failed";

		static void GenerateSimpleTestCase (StreamWriter writer, string testCaseName, string signature, string body, string shouldCompile, string execArg, string expected) {
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    std::shared_ptr<std::stringstream> source = buildSimple(\"" + signature + "\", \"\",\"" + body + "\", \"\");\n" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n";

			writer.WriteLine(source);
		}

		static void GenerateSigTest(StreamWriter writer, string testCaseName, int argCount, string compArg, string shouldCompile, string execArg, string expected, int mod = 0) {
			string s = string.Empty;
			for (int i = 0; i < argCount; i++) {
				s += "Int->";
			}
			s += "Int";

			string source =
				"void Test::" + testCaseName + "() {\n";

			string[] args = new string[] {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"};

			source += "    shared_ptr<std::stringstream> source = buildSimple(\"" + s + "\", \""; //);\n";
			for (int i = 0; i < argCount + mod; i++) {
				source += " " + args [i];
			}

			source += "\", \"2\", \"" + compArg + "\");\n";

			source += "    bool compStatus = compileChecker(source);\n";
			source += "    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n";
			source += "    if (compStatus == true) {\n";
			source += "        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n";
			source += "        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n";
			source += "    }\n";
			source += "}\n";

			writer.WriteLine (source);
		}

		static void GenerateCaseTest(StreamWriter writer, string testCaseName, int caseCount, string compArgs, string shouldCompile, string execArg, string expected) {
			string source = "void Test::" + testCaseName + "() {\n";
			source += "    std::vector<std::string> pattern;\n";

			for (int i = 0; i < caseCount; i++) {
				source += "    pattern.push_back(\"" + i + "\");\n";
			}

			source += "    pattern.push_back(\"n\");\n";

			source += "    std::shared_ptr<std::stringstream> source = buildMultiCase(\"Int\", \"Int\", pattern, \"2\", \"" + compArgs + "\");\n";
			source += "    bool compStatus = compileChecker(source);\n";
			source += "    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n";
			source += "    if(compStatus == true) {\n";
			source += "        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n";
			source += "        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n";
			source += "    }\n";
			source += "}\n";

			writer.WriteLine (source);
		}

		static void GenerateThreePartTest(StreamWriter writer, string testCaseName, string signature, string pattern, string body, string compArg, string shouldCompile, string execArg, string expected) {
			string source = 
				"void Test::" + testCaseName + "() {\n" +
				"    std::shared_ptr<std::stringstream> source = buildSimple(\"" + signature + "\", \"" + pattern + "\", \"" + body + "\", \"" + compArg + "\");\n" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n" +
				"    if(compStatus) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n";

			writer.WriteLine (source);
		}

		static void GenerateFuncTest(StreamWriter writer, string testCaseName, 
			string mainSignature, string mainPattern, string mainBody, 
			string func1Signature, string func1Pattern, string func1Body, 
			string shouldCompile, string execArg, string expected) {

			string source = 
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildFunc(\n" +
				"        \"" + mainSignature + "\", \"" + mainPattern + "\", \"" + mainBody + "\",\n" +
				"        \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\");\n" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n" +
				"    if(compStatus) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n\n";
			writer.WriteLine (source);
		}

		static void GenerateFuncTest(StreamWriter writer, string testCaseName, 
			string mainSignature, string mainPattern, string mainBody, 
			string func1Signature, string func1Pattern, string func1Body, 
			string func2Signature, string func2Pattern, string func2Body,
			string shouldCompile, string execArg, string expected) {

			string source = 
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildFunc(\n" +
				"        \"" + mainSignature + "\", \"" + mainPattern + "\", \"" + mainBody + "\",\n" +
				"        \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\",\n" +
				"        \"" + func2Signature + "\", \"" + func2Pattern + "\", \"" + func2Body + "\");\n" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n" +
				"    if(compStatus) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n\n";
			writer.WriteLine (source);
		}

		static void GenerateFuncTest(StreamWriter writer, string testCaseName, 
			string mainSignature, string mainPattern, string mainBody, 
			string func1Signature, string func1Pattern, string func1Body, 
			string func2Signature, string func2Pattern, string func2Body,
			string func3Signature, string func3Pattern, string func3Body,
			string shouldCompile, string execArg, string expected) {

			string source = 
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildFunc(\n" +
				"        \"" + mainSignature + "\", \"" + mainPattern + "\", \"" + mainBody + "\",\n" +
				"        \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\",\n" +
				"        \"" + func2Signature + "\", \"" + func2Pattern + "\", \"" + func2Body + "\",\n" +
				"        \"" + func3Signature + "\", \"" + func3Pattern + "\", \"" + func3Body + "\");\n" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\"" + compMsg + "\", compStatus == " + shouldCompile + ");\n" +
				"    if(compStatus) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n\n";
			writer.WriteLine (source);
		}


		static void buildAddTest() {
			StreamWriter writer = new StreamWriter ("AddTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "addIntPosPos", "Int", "2+2", "true", "", "4");
			GenerateSimpleTestCase (writer, "addIntZeroZero", "Int", "0+0", "true", "", "0");
			GenerateSimpleTestCase (writer, "addIntPosNeg", "Int", "2+-2", "true", "", "0");
			GenerateSimpleTestCase (writer, "addIntNegPos", "Int", "-2+2", "true", "", "0");
			GenerateSimpleTestCase (writer, "addIntNegNeg", "Int", "-2+-2", "true", "", "-4");

			GenerateSimpleTestCase (writer, "addFloatPosPos", "Float", "2.0+2.0", "true", "", "4.000000");
			GenerateSimpleTestCase (writer, "addFloatZeroZero", "Float", "0.0+0.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "addFloatPosNeg", "Float", "2.0+-2.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "addFloatNegPos", "Float", "-2.0+2.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "addFloatNegNeg", "Float", "-2.0+-2.0", "true", "", "-4.000000");

			GenerateSimpleTestCase (writer, "addBoolTrueTrue", "Bool", "True+True", "false", "", "");
			GenerateSimpleTestCase (writer, "addBoolTrueFalse", "Bool", "True+False", "false", "", "");
			GenerateSimpleTestCase (writer, "addBoolFalseTrue", "Bool", "False+True", "false", "", "");
			GenerateSimpleTestCase (writer, "addBoolFalseFalse", "Bool", "False+False", "false", "", "");

			GenerateSimpleTestCase (writer, "addCharStrStr", "String", "\\\"string\\\"+\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "addCharStrCha", "String", "\\\"string\\\"+'c'", "false", "", "");
			GenerateSimpleTestCase (writer, "addCharChaStr", "Char", "'c'+\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "addCharChaCha", "Char", "'c'+'c'", "false", "", "");

			GenerateSimpleTestCase (writer, "addMixFloatInt", "Float", "2-0+2", "false", "", "");
			GenerateSimpleTestCase (writer, "addMixIntFloat", "Int", "2+2.0", "false", "", "");

			writer.Close ();
		}

		static void buildSubTest() {
			StreamWriter writer = new StreamWriter ("SubTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "subIntPosPos", "Int", "2-2", "true", "", "0");
			GenerateSimpleTestCase (writer, "subIntZeroZero", "Int", "0-0", "true", "", "0");
			GenerateSimpleTestCase (writer, "subIntPosNeg", "Int", "2--2", "true", "", "4");
			GenerateSimpleTestCase (writer, "subIntNegPos", "Int", "-2-2", "true", "", "-4");
			GenerateSimpleTestCase (writer, "subIntNegNeg", "Int", "-2--2", "true", "", "0");

			GenerateSimpleTestCase (writer, "subFloatPosPos", "Float", "2.0-2.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "subFloatZeroZero", "Float", "0.0-0.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "subFloatPosNeg", "Float", "2.0--2.0", "true", "", "4.000000");
			GenerateSimpleTestCase (writer, "subFloatNegPos", "Float", "-2.0-2.0", "true", "", "-4.000000");
			GenerateSimpleTestCase (writer, "subFloatNegNeg", "Float", "-2.0--2.0", "true", "", "0.000000");

			GenerateSimpleTestCase (writer, "subBoolTrueTrue", "Bool", "True-True", "false", "", "");
			GenerateSimpleTestCase (writer, "subBoolTrueFalse", "Bool", "True-False", "false", "", "");
			GenerateSimpleTestCase (writer, "subBoolFalseTrue", "Bool", "False-True", "false", "", "");
			GenerateSimpleTestCase (writer, "subBoolFalseFalse", "Bool", "False-False", "false", "", "");

			GenerateSimpleTestCase (writer, "subCharStrStr", "String", "\\\"string\\\"-\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "subCharStrCha", "String", "\\\"string\\\"-'c'", "false", "", "");
			GenerateSimpleTestCase (writer, "subCharChaStr", "Char", "'c'-\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "subCharChaCha", "Char", "'c'-'c'", "false", "", "");

			GenerateSimpleTestCase (writer, "subMixFloatInt", "Float", "2-0-2", "false", "", "");
			GenerateSimpleTestCase (writer, "subMixIntFloat", "Int", "2-2.0", "false", "", "");

			writer.Close ();
		}

		static void buildMulTest() {
			StreamWriter writer = new StreamWriter ("MulTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "mulIntPosPos", "Int", "2*2", "true", "", "4");
			GenerateSimpleTestCase (writer, "mulIntZeroZero", "Int", "0*0", "true", "", "0");
			GenerateSimpleTestCase (writer, "mulIntPosNeg", "Int", "2*-2", "true", "", "-4");
			GenerateSimpleTestCase (writer, "mulIntNegPos", "Int", "-2*2", "true", "", "-4");
			GenerateSimpleTestCase (writer, "mulIntNegNeg", "Int", "-2*-2", "true", "", "4");

			GenerateSimpleTestCase (writer, "mulFloatPosPos", "Float", "2.0*2.0", "true", "", "4.000000");
			GenerateSimpleTestCase (writer, "mulFloatZeroZero", "Float", "0.0*0.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "mulFloatPosNeg", "Float", "2.0*-2.0", "true", "", "-4.000000");
			GenerateSimpleTestCase (writer, "mulFloatNegPos", "Float", "-2.0*2.0", "true", "", "-4.000000");
			GenerateSimpleTestCase (writer, "mulFloatNegNeg", "Float", "-2.0*-2.0", "true", "", "4.000000");

			GenerateSimpleTestCase (writer, "mulBoolTrueTrue", "Bool", "True*True", "false", "", "");
			GenerateSimpleTestCase (writer, "mulBoolTrueFalse", "Bool", "True*False", "false", "", "");
			GenerateSimpleTestCase (writer, "mulBoolFalseTrue", "Bool", "False*True", "false", "", "");
			GenerateSimpleTestCase (writer, "mulBoolFalseFalse", "Bool", "False*False", "false", "", "");

			GenerateSimpleTestCase (writer, "mulCharStrStr", "String", "\\\"string\\\"*\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "mulCharStrCha", "String", "\\\"string\\\"*'c'", "false", "", "");
			GenerateSimpleTestCase (writer, "mulCharChaStr", "Char", "'c'*\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "mulCharChaCha", "Char", "'c'*'c'", "false", "", "");

			GenerateSimpleTestCase (writer, "mulMixFloatInt", "Float", "2-0*2", "false", "", "");
			GenerateSimpleTestCase (writer, "mulMixIntFloat", "Int", "2*2.0", "false", "", "");

			writer.Close ();
		}

		static void buildDivTest() {
			StreamWriter writer = new StreamWriter ("DivTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "divIntPosPos", "Int", "2/2", "true", "", "1");
			GenerateSimpleTestCase (writer, "divIntZeroZero", "Int", "0/0", "true", "", "0");
			GenerateSimpleTestCase (writer, "divIntPosNeg", "Int", "2/-2", "true", "", "-1");
			GenerateSimpleTestCase (writer, "divIntNegPos", "Int", "-2/2", "true", "", "-1");
			GenerateSimpleTestCase (writer, "divIntNegNeg", "Int", "-2/-2", "true", "", "1");

			GenerateSimpleTestCase (writer, "divFloatPosPos", "Float", "2.0/2.0", "true", "", "1.000000");
			GenerateSimpleTestCase (writer, "divFloatZeroZero", "Float", "0.0/0.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "divFloatPosNeg", "Float", "2.0/-2.0", "true", "", "-1.000000");
			GenerateSimpleTestCase (writer, "divFloatNegPos", "Float", "-2.0/2.0", "true", "", "-1.000000");
			GenerateSimpleTestCase (writer, "divFloatNegNeg", "Float", "-2.0/-2.0", "true", "", "1.000000");

			GenerateSimpleTestCase (writer, "divBoolTrueTrue", "Bool", "True/True", "false", "", "");
			GenerateSimpleTestCase (writer, "divBoolTrueFalse", "Bool", "True/False", "false", "", "");
			GenerateSimpleTestCase (writer, "divBoolFalseTrue", "Bool", "False/True", "false", "", "");
			GenerateSimpleTestCase (writer, "divBoolFalseFalse", "Bool", "False/False", "false", "", "");

			GenerateSimpleTestCase (writer, "divCharStrStr", "String", "\\\"string\\\"/\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "divCharStrCha", "String", "\\\"string\\\"/'c'", "false", "", "");
			GenerateSimpleTestCase (writer, "divCharChaStr", "Char", "'c'/\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "divCharChaCha", "Char", "'c'/'c'", "false", "", "");

			GenerateSimpleTestCase (writer, "divMixFloatInt", "Float", "2-0/2", "false", "", "");
			GenerateSimpleTestCase (writer, "divMixIntFloat", "Int", "2/2.0", "false", "", "");

			writer.Close ();
		}

		static void buildModTest() {
			StreamWriter writer = new StreamWriter ("ModTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "modIntPosPos", "Int", "2%2", "true", "", "0");
			GenerateSimpleTestCase (writer, "modIntZeroZero", "Int", "0%0", "true", "", "0");
			GenerateSimpleTestCase (writer, "modIntPosNeg", "Int", "2%-2", "true", "", "0");
			GenerateSimpleTestCase (writer, "modIntNegPos", "Int", "-2%2", "true", "", "0");
			GenerateSimpleTestCase (writer, "modIntNegNeg", "Int", "-2%-2", "true", "", "0");

			GenerateSimpleTestCase (writer, "modFloatPosPos", "Float", "2.0%2.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "modFloatZeroZero", "Float", "0.0%0.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "modFloatPosNeg", "Float", "2.0%-2.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "modFloatNegPos", "Float", "-2.0%2.0", "true", "", "0.000000");
			GenerateSimpleTestCase (writer, "modFloatNegNeg", "Float", "-2.0%-2.0", "true", "", "0.000000");

			GenerateSimpleTestCase (writer, "modBoolTrueTrue", "Bool", "True%True", "false", "", "");
			GenerateSimpleTestCase (writer, "modBoolTrueFalse", "Bool", "True%False", "false", "", "");
			GenerateSimpleTestCase (writer, "modBoolFalseTrue", "Bool", "False%True", "false", "", "");
			GenerateSimpleTestCase (writer, "modBoolFalseFalse", "Bool", "False%False", "false", "", "");

			GenerateSimpleTestCase (writer, "modCharStrStr", "String", "\\\"string\\\"%\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "modCharStrCha", "String", "\\\"string\\\"%'c'", "false", "", "");
			GenerateSimpleTestCase (writer, "modCharChaStr", "Char", "'c'%\\\"string\\\"", "false", "", "");
			GenerateSimpleTestCase (writer, "modCharChaCha", "Char", "'c'%'c'", "false", "", "");

			GenerateSimpleTestCase (writer, "modMixFloatInt", "Float", "2-0%2", "false", "", "");
			GenerateSimpleTestCase (writer, "modMixIntFloat", "Int", "2%2.0", "false", "", "");

			writer.Close ();
		}

		static void buildBinBoolTest() {
			StreamWriter writer = new StreamWriter ("BinBoolTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "binEqualTrueTrue", "Bool", "True==True", "true", "", "True");
			GenerateSimpleTestCase (writer, "binEqualTrueFalse", "Bool", "True==False", "true", "", "False");
			GenerateSimpleTestCase (writer, "binEqualFalseTrue", "Bool", "False==True", "true", "", "False");
			GenerateSimpleTestCase (writer, "binEqualFalseFalse", "Bool", "False==False", "true", "", "True");

			GenerateSimpleTestCase (writer, "binNotEqualTrueTrue", "Bool", "True!=True", "true", "", "False");
			GenerateSimpleTestCase (writer, "binNotEqualTrueFalse", "Bool", "True!=False", "true", "", "True");
			GenerateSimpleTestCase (writer, "binNotEqualFalseTrue", "Bool", "False!=True", "true", "", "True");
			GenerateSimpleTestCase (writer, "binNotEqualFalseFalse", "Bool", "False!=False", "true", "", "False");

			GenerateSimpleTestCase (writer, "binAndTrueTrue", "Bool", "True&&True", "true", "", "True");
			GenerateSimpleTestCase (writer, "binAndTrueFalse", "Bool", "True&&False", "true", "", "False");
			GenerateSimpleTestCase (writer, "binAndFalseTrue", "Bool", "False&&True", "true", "", "False");
			GenerateSimpleTestCase (writer, "binAndFalseFalse", "Bool", "False&&False", "true", "", "False");

			GenerateSimpleTestCase (writer, "binOrTrueTrue", "Bool", "True||True", "true", "", "True");
			GenerateSimpleTestCase (writer, "binOrTrueFalse", "Bool", "True||False", "true", "", "True");
			GenerateSimpleTestCase (writer, "binOrFalseTrue", "Bool", "False||True", "true", "", "True");
			GenerateSimpleTestCase (writer, "binOrFalseFalse", "Bool", "False||False", "true", "", "False");

			GenerateSimpleTestCase (writer, "binGrEqTrueTrue", "Bool", "True>=True", "false", "", "");
			GenerateSimpleTestCase (writer, "binGrEqTrueFalse", "Bool", "True>=False", "false", "", "");
			GenerateSimpleTestCase (writer, "binGrEqFalseTrue", "Bool", "False>=True", "false", "", "");
			GenerateSimpleTestCase (writer, "binGrEqFalseFalse", "Bool", "False>=False", "false", "", "");

			GenerateSimpleTestCase (writer, "binLeEqTrueTrue", "Bool", "True<=True", "false", "", "");
			GenerateSimpleTestCase (writer, "binLeEqTrueFalse", "Bool", "True<=False", "false", "", "");
			GenerateSimpleTestCase (writer, "binLeEqFalseTrue", "Bool", "False<=True", "false", "", "");
			GenerateSimpleTestCase (writer, "binLeEqFalseFalse", "Bool", "False<=False", "false", "", "");

			GenerateSimpleTestCase (writer, "binGreatTrueTrue", "Bool", "True>True", "false", "", "");
			GenerateSimpleTestCase (writer, "binGreatTrueFalse", "Bool", "True>False", "false", "", "");
			GenerateSimpleTestCase (writer, "binGreatFalseTrue", "Bool", "False>True", "false", "", "");
			GenerateSimpleTestCase (writer, "binGreatFalseFalse", "Bool", "False>False", "false", "", "");

			GenerateSimpleTestCase (writer, "binLessTrueTrue", "Bool", "True<True", "false", "", "");
			GenerateSimpleTestCase (writer, "binLessTrueFalse", "Bool", "True<False", "false", "", "");
			GenerateSimpleTestCase (writer, "binLessFalseTrue", "Bool", "False<True", "false", "", "");
			GenerateSimpleTestCase (writer, "binLessFalseFalse", "Bool", "False<False", "false", "", "");

			writer.Close ();
		}

		static void buildBinValTest() {
			StreamWriter writer = new StreamWriter ("BinValTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSimpleTestCase (writer, "binEqualIntInt", "Bool", "2==2", "true", "", "True");
			GenerateSimpleTestCase (writer, "binNotEqualIntInt", "Bool", "2!=2", "true", "", "False");
			GenerateSimpleTestCase (writer, "binAndIntInt", "Bool", "2&&2", "false", "", "");
			GenerateSimpleTestCase (writer, "binOrIntInt", "Bool", "2||2", "false", "", "");
			GenerateSimpleTestCase (writer, "binGrEqIntInt", "Bool", "2>=2", "true", "", "True");
			GenerateSimpleTestCase (writer, "binLeEqIntInt", "Bool", "2<=2", "true", "", "True");
			GenerateSimpleTestCase (writer, "binGreatIntInt", "Bool", "2>2", "true", "", "False");
			GenerateSimpleTestCase (writer, "binLessIntInt", "Bool", "2<2", "true", "", "False");

			GenerateSimpleTestCase (writer, "binEqualFloatFloat", "Bool", "2.0==2.0", "true", "", "True");
			GenerateSimpleTestCase (writer, "binNotEqualFloatFloat", "Bool", "2.0!=2.0", "true", "", "False");
			GenerateSimpleTestCase (writer, "binAndFloatFloat", "Bool", "2.0&&2.0", "false", "", "");
			GenerateSimpleTestCase (writer, "binOrFloatFloat", "Bool", "2.0||2.0", "false", "", "");
			GenerateSimpleTestCase (writer, "binGrEqFloatFloat", "Bool", "2.0>=2.0", "true", "", "True");
			GenerateSimpleTestCase (writer, "binLeEqFloatFloat", "Bool", "2.0<=2.0", "true", "", "True");
			GenerateSimpleTestCase (writer, "binGreatFloatFloat", "Bool", "2.0>2.0", "true", "", "False");
			GenerateSimpleTestCase (writer, "binLessFloatFloat", "Bool", "2.0<2.0", "true", "", "False");

			writer.Close ();
		}

		static void buildSignatureTest() {
			StreamWriter writer = new StreamWriter ("SigTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateSigTest (writer, "sigLengthZeroInt", 0, "", "true", "", "2");
			GenerateSigTest (writer, "sigLengthOneInt", 1, "1", "true", "", "2");
			GenerateSigTest (writer, "sigLengthTwoInt", 2, "1, 2", "true", "", "2");
			GenerateSigTest (writer, "sigLengthThreeInt", 3, "1, 2, 3", "true", "", "2");
			GenerateSigTest (writer, "sigLengthFourInt", 4, "1, 2, 3, 4", "true", "", "2");
			GenerateSigTest (writer, "sigLengthFiveInt", 5, "1, 2, 3, 4, 5", "true", "", "2");
			GenerateSigTest (writer, "sigLengthSixInt", 6, "1, 2, 3, 4, 5, 6", "true", "", "2");
			GenerateSigTest (writer, "sigLengthSevenInt", 7, "1, 2, 3, 4, 5, 6, 7", "true", "", "2");
			GenerateSigTest (writer, "sigLengthEightInt", 8, "1, 2, 3, 4, 5, 6, 7, 8", "true", "", "2");
			GenerateSigTest (writer, "sigLengthNineInt", 9, "1, 2, 3, 4, 5, 6, 7, 8, 9", "true", "", "2");
			GenerateSigTest (writer, "sigLengthTenInt", 10, "1, 2, 3, 4, 5, 6, 7, 8, 9, 10", "true", "", "2");

			GenerateSigTest (writer, "patLengthSameInt", 2, "1, 2", "true", "", "2");
			GenerateSigTest (writer, "patLengthLessInt", 2, "1", "false", "", "", -1);
			GenerateSigTest (writer, "patLengthMoreInt", 2, "1, 2, 3", "false", "", "", 1);

			writer.Close ();
		}

		static void buildCaseTest() {
			StreamWriter writer = new StreamWriter ("CasTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateCaseTest (writer, "casCountOneInt", 1, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountTwoInt", 2, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountThreeInt", 3, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountFourInt", 4, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountFiveInt", 5, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountSixInt", 6, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountSevenInt", 7, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountEightInt", 8, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountNineInt", 9, "1", "true", "", "2");
			GenerateCaseTest (writer, "casCountTenInt", 10, "1", "true", "", "2");

			writer.Close ();
		}

		static void buildListTest() {
			StreamWriter writer = new StreamWriter ("ListTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateThreePartTest (writer, "listCasEmpty", "[Int]->Int", "[]", "2", "[]", "true", "", "2");
			GenerateThreePartTest (writer, "listAppendEmpty", "[Int]", "", "1:[]", "", "true", "", "[1]");
			GenerateThreePartTest (writer, "listInt", "[Int]", "", "[2]", "", "true", "", "[2]");
			GenerateThreePartTest (writer, "listFloat", "[Float]", "", "[2.0]", "", "true", "", "[2.000000]");
			GenerateThreePartTest (writer, "listBool", "[Bool]", "", "[True]", "", "true", "", "[True]");
			GenerateThreePartTest (writer, "listChar", "[Char]", "", "['c']", "", "true", "", "['c']");
			GenerateThreePartTest (writer, "listString", "[String]", "", "[\\\"string\\\"]", "", "true", "", "[\\\"string\\\"]");

			GenerateThreePartTest (writer, "listListInt", "[[Int]]", "", "[[2]]", "", "true", "", "[[2]]");
			GenerateThreePartTest (writer, "listListFloat", "[[Float]]", "", "[[2.0]]", "", "true", "", "[[2.000000]]");
			GenerateThreePartTest (writer, "listListBool", "[[Bool]]", "", "[[True]]", "", "true", "", "[[True]]");
			GenerateThreePartTest (writer, "listListChar", "[[Char]]", "", "[['c']]", "", "true", "", "[['c']]");
			GenerateThreePartTest (writer, "listListString", "[[String]]", "", "[[\\\"string\\\"]]", "", "true", "", "[[\\\"string\\\"]]");

			GenerateThreePartTest (writer, "listListListInt", "[[[Int]]]", "", "[[[2]]]", "", "true", "", "[[[2]]]");

			GenerateThreePartTest (writer, "listAppendInt", "[Int]", "", "1:[2,3]", "", "true", "", "[1, 2, 3]");
			GenerateThreePartTest (writer, "listAppendFloat", "[Float]", "", "1.0:[2.0,3.0]", "", "true", "", "[1.000000, 2.000000, 3.000000]");
			GenerateThreePartTest (writer, "listAppendBool", "[Bool]", "", "True:[False,True]", "", "true", "", "[True, False, True]");
			GenerateThreePartTest (writer, "listAppendChar", "[Char]", "", "'c':['h','a','r']", "", "true", "", "['c', 'h', 'a', 'r']");
			GenerateThreePartTest (writer, "listAppendString", "[String]", "", "\\\"string\\\":[\\\"string\\\",\\\"string\\\"]", "", "true", "", "[\\\"string\\\", \\\"string\\\", \\\"string\\\"]");

			GenerateThreePartTest (writer, "listLongInt", "[Int]", "", "[0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]", "", "true", "", "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0]");

			GenerateThreePartTest (writer, "listFloatInInt", "[Int]", "", "[2.0]", "", "false", "", "");
			GenerateThreePartTest (writer, "listBoolInInt", "[Int]", "", "[True]", "", "false", "", "");
			GenerateThreePartTest (writer, "listCharInInt", "[Int]", "", "['c']", "", "false", "", "");
			GenerateThreePartTest (writer, "listStringInInt", "[Int]", "", "[\\\"string\\\"]", "", "false", "", "");

			GenerateThreePartTest (writer, "listIntInFloat", "[Float]", "", "[2]", "", "false", "", "");
			GenerateThreePartTest (writer, "listBoolInFloat", "[Float]", "", "[True]", "", "false", "", "");
			GenerateThreePartTest (writer, "listCharInFloat", "[Float]", "", "['c']", "", "false", "", "");
			GenerateThreePartTest (writer, "listStringInFloat", "[Float]", "", "[\\\"string\\\"]", "", "false", "", "");

			GenerateThreePartTest (writer, "listIntInBool", "[Bool]", "", "[2]", "", "false", "", "");
			GenerateThreePartTest (writer, "listFloatInBool", "[Bool]", "", "[2.0]", "", "false", "", "");
			GenerateThreePartTest (writer, "listCharInBool", "[Bool]", "", "['c']", "", "false", "", "");
			GenerateThreePartTest (writer, "listStringInBool", "[Bool]", "", "[\\\"string\\\"]", "", "false", "", "");

			GenerateThreePartTest (writer, "listIntInChar", "[Char]", "", "[2]", "", "false", "", "");
			GenerateThreePartTest (writer, "listFloatInChar", "[Char]", "", "[2.0]", "", "false", "", "");
			GenerateThreePartTest (writer, "listBoolInChar", "[Char]", "", "[True]", "", "false", "", "");
			GenerateThreePartTest (writer, "listStringInChar", "[Char]", "", "[\\\"True\\\"]", "", "false", "", "");

			GenerateThreePartTest (writer, "listIntInString", "[String]", "", "[2]", "", "false", "", "");
			GenerateThreePartTest (writer, "listFloatInString", "[String]", "", "[2.0]", "", "false", "", "");
			GenerateThreePartTest (writer, "listBoolInString", "[String]", "", "[True]", "", "false", "", "");
			GenerateThreePartTest (writer, "listCharInString", "[String]", "", "['c']", "", "false", "", "");

			GenerateThreePartTest (writer, "listMixTypes", "[Int]", "", "[2,2.0,False,'c',\\\"string\\\"]", "", "false", "", "");

			GenerateThreePartTest (writer, "listNestedDifLengthInt", "[[Int]]", "", "[[1,2,3],[1,2,3,4,5,6,7,8,9,0]]", "", "true", "", "[[1, 2, 3], [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]]");

			GenerateThreePartTest (writer, "listTupleInt", "[(Int, Int)]", "", "[(1,2),(3,4),(5,6)]", "", "true", "", "[(1, 2), (3, 4), (5, 6)]");
			GenerateThreePartTest (writer, "listTupleFloat", "[(Float, Float)]", "", "[(1.0,2.0),(3.0,4.0),(5.0,6.0)]", "", "true", "", "[(1.000000, 2.000000), (3.000000, 4.000000), (5.000000, 6.000000)]");
			GenerateThreePartTest (writer, "listTupleBool", "[(Bool, Bool)]", "", "[(True,False),(False,True),(False,False)]", "", "true", "", "[(True, False), (False, True), (False, False)]");
			GenerateThreePartTest (writer, "listTupleChar", "[(Char, Char)]", "", "[('c','h'),('a','r'),('c','h')]", "", "true", "", "[('c', 'h'), ('a', 'r'), ('c', 'h')]");
			GenerateThreePartTest (writer, "listTupleString", "[(String, String)]", "", "[(\\\"string\\\",\\\"string\\\"),(\\\"string\\\",\\\"string\\\"),(\\\"string\\\",\\\"string\\\")]", "", "true", "", "[(\\\"string\\\", \\\"string\\\"), (\\\"string\\\", \\\"string\\\"), (\\\"string\\\", \\\"string\\\")]");

			writer.Close ();
		}

		static void buildTupleTest() {
			StreamWriter writer = new StreamWriter ("TupleTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateThreePartTest (writer, "tupleEmpty", "()", "", "()", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleEmptyInt", "(Int)", "", "()", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleEmptyFloat", "(Float)", "", "()", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleEmptyBool", "(Bool)", "", "()", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleEmptyChar", "(Char)", "", "()", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleEmptyString", "(String)", "", "()", "", "false", "", "");

			GenerateThreePartTest (writer, "tupleOneInt", "(Int)", "", "(2)", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleOneFloat", "(Float)", "", "(2.0)", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleOneBool", "(Bool)", "", "(True)", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleOneChar", "(Char)", "", "('c')", "", "false", "", "");
			GenerateThreePartTest (writer, "tupleOneString", "(String)", "", "(\\\"string\\\")", "", "false", "", "");

			GenerateThreePartTest (writer, "tupleTwoInt", "(Int,Int)", "", "(2,2)", "", "true", "", "(2, 2)");
			GenerateThreePartTest (writer, "tupleTwoFloat", "(Float,Float)", "", "(2.0,2.0)", "", "true", "", "(2.000000, 2.000000)");
			GenerateThreePartTest (writer, "tupleTwoBool", "(Bool,Bool)", "", "(True,False)", "", "true", "", "(True, False)");
			GenerateThreePartTest (writer, "tupleTwoChar", "(Char, Char)", "", "('c','h')", "", "true", "", "('c', 'h')");
			GenerateThreePartTest (writer, "tupleTwoString", "(String, String)", "", "(\\\"string\\\",\\\"string\\\")", "", "true", "", "(\\\"string\\\", \\\"string\\\")");

			GenerateThreePartTest (writer, "tupleThreeInt", "(Int,Int, Int)", "", "(2,2,2)", "", "true", "", "(2, 2, 2)");
			GenerateThreePartTest (writer, "tupleThreeFloat", "(Float,Float, Float)", "", "(2.0,2.0,2.0)", "", "true", "", "(2.000000, 2.000000, 2.000000)");
			GenerateThreePartTest (writer, "tupleThreeBool", "(Bool,Bool, Bool)", "", "(True, False, True)", "", "true", "", "(True, False, True)");
			GenerateThreePartTest (writer, "tupleThreeChar", "(Char, Char, Char)", "", "('c','h','a')", "", "true", "", "('c', 'h', 'a')");
			GenerateThreePartTest (writer, "tupleThreeString", "(String, String, String)", "", "(\\\"string\\\",\\\"string\\\",\\\"string\\\")", "", "true", "", "(\\\"string\\\", \\\"string\\\", \\\"string\\\")");

			GenerateThreePartTest (writer, "tupleMixed", "(Int, Float, Bool, Char, Char, Char, Char, String)", "", "(2,2.0,True,'c','h','a','r',\\\"string\\\")", "", "true", "", "(2, 2.000000, True, 'c', 'h', 'a', 'r', \\\"string\\\")");

			GenerateThreePartTest (writer, "tupleListInt", "([Int],[Int])", "", "([2],[2])", "", "true", "", "([2], [2])");
			GenerateThreePartTest (writer, "tupleListFloat", "([Float],[Float])", "", "([2.0],[2.0])", "", "true", "", "([2.000000], [2.000000])");
			GenerateThreePartTest (writer, "tupleListBool", "([Bool],[Bool])", "", "([True],[False])", "", "true", "", "([True], [False])");
			GenerateThreePartTest (writer, "tupleListChar", "([Char],[Char])", "", "(['c'],['h'])", "", "true", "", "(['c'], ['h'])");
			GenerateThreePartTest (writer, "tupleListString", "([String],[String])", "", "([\\\"string\\\"],[\\\"string\\\"])", "", "true", "", "([\\\"string\\\"], [\\\"string\\\"])");

			GenerateThreePartTest (writer, "tupleNested", "((Int, Int), (Int,Int))", "", "((1,2),(3,4))", "", "true", "", "((1, 2), (3, 4))");
			GenerateThreePartTest (writer, "tupleTwoNested", "((Int, Float), (Bool, (Char, String)))", "", "((1,2.0),(True,('c',\\\"har\\\")))", "", "true", "", "((1, 2.000000), (True, ('c', \\\"har\\\")))");
			GenerateThreePartTest (writer, "tupleSuperNested", "(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,(Char,Char)))))))))))", "", "('h',('e',('l',('l',('o',(' ',('w',('o',('r',('l',('d','!')))))))))))", "", "true", "", "('h', ('e', ('l', ('l', ('o', (' ', ('w', ('o', ('r', ('l', ('d', '!')))))))))))");
			GenerateThreePartTest (writer, "tupleWrongType", "(Int,Float,Bool,Char,String)", "", "(1.2,True,'c',\\\"string\\\",1)", "", "false", "", "");

			writer.Close ();
		}

		static void buildFuncTest() {
			StreamWriter writer = new StreamWriter ("FuncTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateFuncTest (writer, "funcRetInt", "Int", "", "func()", "Int", "", "2", "true", "", "2");
			GenerateFuncTest (writer, "funcRetFloat", "Float", "", "func()", "Float", "", "2.0", "true", "", "2.000000");
			GenerateFuncTest (writer, "funcRetBool", "Bool", "", "func()", "Bool", "", "True", "true", "", "True");
			GenerateFuncTest (writer, "funcRetChar", "Char", "", "func()", "Char", "", "'c'", "true", "", "'c'");
			GenerateFuncTest (writer, "funcRetString", "String", "", "func()", "String", "", "\\\"string\\\"", "true", "", "\\\"string\\\"");

			GenerateFuncTest (writer, "funcIntRetInt", "Int", "", "func(2)", "Int->Int", "n", "n", "true", "", "2");
			GenerateFuncTest (writer, "funcFloatRetFloat", "Float", "", "func(2.0)", "Float->Float", "n", "n", "true", "", "2.000000");
			GenerateFuncTest (writer, "funcBoolRetBool", "Bool", "", "func(True)", "Bool->Bool", "n", "n", "true", "", "True");
			GenerateFuncTest (writer, "funcCharRetChar", "Char", "", "func('c')", "Char->Char", "n", "n", "true", "", "'c'");
			GenerateFuncTest (writer, "funcStringRetString", "String", "", "func(\\\"string\\\")", "String->String", "n", "n", "true", "", "\\\"string\\\"");

			GenerateFuncTest (writer, "funcIntIntRetInt", "Int", "", "func(2,2)", "Int->Int->Int", "m n", "m+n", "true", "", "4");
			GenerateFuncTest (writer, "funcFloatFloatRetFloat", "Float", "", "func(2.0,2.0)", "Float->Float->Float", "m n", "m+n", "true", "", "4.000000");
			GenerateFuncTest (writer, "funcBoolBoolRetBool", "Bool", "", "func(True, False)", "Bool->Bool->Bool", "m n", "m||n", "true", "", "True");
			GenerateFuncTest (writer, "funcCharCharRetChar", "Char", "", "func('c','h')", "Char->Char->Char", "m n", "'c'", "true", "", "'c'");
			GenerateFuncTest (writer, "funcStringStringRetString", "String", "", "func(\\\"string\\\", \\\"string\\\")", "String->String->String", "m n", "\\\"string\\\"", "true", "", "\\\"string\\\"");

			GenerateFuncTest (writer, "funcListIntRetListInt", "[Int]", "", "func([1])", "[Int]->[Int]", "n", "n", "true", "", "[1]");
			GenerateFuncTest (writer, "funcListFloatRetListFloat", "[Float]", "", "func([1.0])", "[Float]->[Float]", "n", "n", "true", "", "[1.000000]");
			GenerateFuncTest (writer, "funcListBoolRetListBool", "[Bool]", "", "func([True])", "[Bool]->[Bool]", "n", "n", "true", "", "[True]");
			GenerateFuncTest (writer, "funcListCharRetListChar", "[Char]", "", "func(['c'])", "[Char]->[Char]", "n", "n", "true", "", "['c']");
			GenerateFuncTest (writer, "funcListStringRetListString", "[String]", "", "func([\\\"string\\\"])", "[String]->[String]", "n", "n", "true", "", "[\\\"string\\\"]");

			GenerateFuncTest (writer, "funcTupleIntIntRetTupleIntInt", "(Int,Int)", "", "func((2,2))", "(Int,Int)->(Int,Int)", "n", "n", "true", "", "(2, 2)");
			GenerateFuncTest (writer, "funcTupleFloatFloatRetTupleFloatFloat", "(Float,Float)", "", "func((2.0,2.0))", "(Float,Float)->(Float,Float)", "n", "n", "true", "", "(2.000000, 2.000000)");
			GenerateFuncTest (writer, "funcTupleBoolBoolRetTupleBoolBool", "(Bool,Bool)", "", "func((True,False))", "(Bool,Bool)->(Bool,Bool)", "n", "n", "true", "", "(True, False)");
			GenerateFuncTest (writer, "funcTupleCharCharRetTupleCharChar", "(Char,Char)", "", "func(('c','h'))", "(Char,Char)->(Char,Char)", "n", "n", "true", "", "('c', 'h')");
			GenerateFuncTest (writer, "funcTupleStringStringRetTupleStringString", "(String,String)", "", "func((\\\"string\\\",\\\"string\\\"))", "(String,String)->(String,String)", "n", "n", "true", "", "(\\\"string\\\", \\\"string\\\")");

			GenerateFuncTest (writer, "funcIntToIntRetInt", "Int", "", "func1(func2)", "(Int->Int)->Int", "f", "f(2)", "Int->Int", "n", "n", "true", "", "2");
			GenerateFuncTest (writer, "funcFloatToFloatRetFloat", "Float", "", "func1(func2)", "(Float->Float)->Float", "f", "f(2.0)", "Float->Float", "n", "n", "true", "", "2.000000");
			GenerateFuncTest (writer, "funcBoolToBoolRetBool", "Bool", "", "func1(func2)", "(Bool->Bool)->Bool", "f", "f(True)", "Bool->Bool", "n", "n", "true", "", "True");
			GenerateFuncTest (writer, "funcCharToCharRetChar", "Char", "", "func1(func2)", "(Char->Char)->Char", "f", "f('c')", "Char->Char", "n", "n", "true", "", "'c'");
			GenerateFuncTest (writer, "funcStringToStringRetString", "String", "", "func1(func2)", "(String->String)->String", "f", "f(\\\"string\\\")", "String->String", "n", "n", "true", "", "\\\"string\\\"");

			GenerateFuncTest (writer, "funcListIntToListIntRetListInt", "[Int]", "", "func1(func2)", "([Int]->[Int])->[Int]", "f", "f([1])", "[Int]->[Int]", "n", "n", "true", "", "[1]");
			GenerateFuncTest (writer, "funcTupleIntIntToTupleIntIntRetInt", "(Int,Int)", "", "func1(func2)", "((Int,Int)->(Int,Int))->(Int,Int)", "f", "f((1,2))", "(Int,Int)->(Int,Int)", "n", "n", "true", "", "(1, 2)");
			GenerateFuncTest (writer, "funcIntToIntRetToIntRetToInt", "Int", "", "func1(func2)", "((Int->Int)->Int)->Int", "f", "f(func3)", "(Int->Int)->Int", "f", "f(2)", "Int->Int", "n", "n", "true", "", "2");

			writer.Close ();
		}

		static void buildKeywordTest() {
			StreamWriter writer = new StreamWriter ("KeywordTest.cpp");
			writer.WriteLine ("#include \"Test.h\"\n\n");

			GenerateThreePartTest (writer, "keywordInt", "Int->Int", "Int", "2", "2", "false", "", "");
			GenerateThreePartTest (writer, "keywordFloat", "Float->Float", "Float", "2.0", "2.0","false", "", "");
			GenerateThreePartTest (writer, "keywordBool", "Bool->Bool", "Bool", "True", "True", "false", "", "");
			GenerateThreePartTest (writer, "keywordChar", "Char->Char", "Char", "'c'", "'c'", "false", "", "");
			GenerateThreePartTest (writer, "keywordString", "String->String", "String", "\\\"string\\\"", "\\\"string\\\"", "false", "", "");
			GenerateThreePartTest (writer, "keywordDef", "Int->Int", "def", "2", "2", "false", "", "");
			GenerateThreePartTest (writer, "keywordMain", "Int->Int", "main", "2", "2", "false", "", "");

			GenerateThreePartTest (writer, "comment", "Int", "", "2 # This is a comment", "", "true", "", "2");

			writer.Close ();
		}

		public static void Main (string[] args)
		{
			buildAddTest ();
			buildSubTest ();
			buildMulTest ();
			buildDivTest ();
			buildModTest ();
			buildBinBoolTest ();
			buildBinValTest ();
			buildSignatureTest ();
			buildCaseTest ();
			buildListTest ();
			buildTupleTest ();
			buildFuncTest ();
			buildKeywordTest ();
		}
	}
}
