using System;
using System.IO;

namespace TestGenerator
{
	partial class Test
	{
		static string compMsg = "Compilation failed";
		static string execMsg = "Execution failed";

		static StreamWriter testWriter = new StreamWriter("ZZcpptest.txt");
		static StreamWriter headWriter = new StreamWriter("ZZcppheader.txt");

		static void Append(string name) {
			testWriter.WriteLine ("CPPUNIT_TEST(" + name + ");");
			headWriter.WriteLine ("void " + name + "();");
		}

		static void AppendTop(string name) {
			testWriter.WriteLine ("\n// " + name);
			headWriter.WriteLine ("\n// " + name);
		}

		static void GenerateSimpleTestCase (StreamWriter writer, string testCaseName, string signature, string body, string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
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
			Append (testCaseName);
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
			Append (testCaseName);
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
			Append (testCaseName);
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
			string retType, 
			string func1Signature, string func1Pattern, string func1Body, string func1Arg,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source = 
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildFunc(\"" + retType + "\",\n" +
				"        \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\", \"" + func1Arg + "\");\n" +
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
			string retType, 
			string func1Signature, string func1Pattern, string func1Body, string func1Arg,
			string func2Signature, string func2Pattern, string func2Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source = 
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildFunc(\"" + retType + "\",\n" +
				"        \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\", \"" + func1Arg + "\",\n" +
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
			string retType, 
			string func1Signature, string func1Pattern, string func1Body, string func1Arg,
			string func2Signature, string func2Pattern, string func2Body,
			string func3Signature, string func3Pattern, string func3Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source = 
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildFunc(\"" + retType + "\",\n" +
				"        \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\", \"" + func1Arg + "\",\n" +
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

		static void GenerateNameFuncTest(StreamWriter writer, string testCaseName, string retType,
			string func1Name, string func1Signature, string func1Pattern, string func1Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildNamedFunc(\"" + retType + "\",\n" +
				"        \"" + func1Name + "\", \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\");" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\\\"" + compMsg + "\\\", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n\n";

			writer.WriteLine (source);
		}

		static void GenerateNameFuncTest(StreamWriter writer, string testCaseName, string retType,
			string func1Name, string func1Signature, string func1Pattern, string func1Body,
			string func2Name, string func2Signature, string func2Pattern, string func2Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + " () {\n" +
				"    std::shared_ptr<std::stringstream> source = buildNamedFunc(\"" + retType + "\",\n" +
				"        \"" + func1Name + "\", \"" + func1Signature + "\", \"" + func1Pattern + "\", \"" + func1Body + "\",\n" +
				"        \"" + func2Name + "\", \"" + func2Signature + "\", \"" + func2Pattern + "\", \"" + func2Body + "\");\n" +
				"    bool compStatus = compileChecker(source);\n" +
				"    CPPUNIT_ASSERT_MESSAGE(\\\"" + compMsg + "\\\", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(\"" + execMsg + "\", execStatus);\n" +
				"    }\n" +
				"}\n\n";

			writer.WriteLine (source);
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
			buildScopeTest ();
			buildPrecedenceTest ();

			headWriter.Close ();
			testWriter.Close ();
		}
	}
}
