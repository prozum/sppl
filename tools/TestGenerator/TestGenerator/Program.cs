using System;
using System.IO;

namespace TestGenerator
{
	partial class Test
	{
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
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
			writer.WriteLine(source);
		}

		static void GenerateSigTest(StreamWriter writer, string testCaseName, int argCount, string compArg, string shouldCompile, string execArg, string expected, int mod = 0) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
			writer.WriteLine (source);
		}

		static void GenerateCaseTest(StreamWriter writer, string testCaseName, int caseCount, string compArgs, string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
			writer.WriteLine (source);
		}

		static void GenerateThreePartTest(StreamWriter writer, string testCaseName, string signature, string pattern, string body, string compArg, string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
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
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
			writer.WriteLine (source);
		}

		static void GenerateFuncTest(StreamWriter writer, string testCaseName, 
			string retType, 
			string func1Signature, string func1Pattern, string func1Body, string func1Arg,
			string func2Signature, string func2Pattern, string func2Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
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
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
			writer.WriteLine (source);
		}

		static void GenerateNameFuncTest(StreamWriter writer, string testCaseName, string retType,
			string func1Name, string func1Signature, string func1Pattern, string func1Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
			writer.WriteLine (source);
		}

		static void GenerateNameFuncTest(StreamWriter writer, string testCaseName, string retType,
			string func1Name, string func1Signature, string func1Pattern, string func1Body,
			string func2Name, string func2Signature, string func2Pattern, string func2Body,
			string shouldCompile, string execArg, string expected) {
			Append (testCaseName);
			string source =
				"void Test::" + testCaseName + "() { \n" +
				"    bool compStatus = compileChecker(\"" + testCaseName + ".sppl\");\n" +
				"    CPPUNIT_ASSERT_MESSAGE(" + (shouldCompile.Equals("true") ? "compMsgFail" : "compMsgSucc") + ", compStatus == " + shouldCompile + ");\n" +
				"    if (compStatus == true) {\n" +
				"        bool execStatus = executeChecker(\"" + execArg + "\", \"" + expected + "\");\n" +
				"        CPPUNIT_ASSERT_MESSAGE(execMsg, execStatus);\n" +
				"    }\n" +
				"}\n";
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
