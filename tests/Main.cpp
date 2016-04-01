#include <iostream>

#include <iostream>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "TestMaster.h"

//*****************************************************
/*
class MathClass {
public:
int adder(int a, int b);
int subber(int a, int b);
};

int MathClass::adder(int a, int b) {
	return a + b;	
}

int MathClass::subber(int a, int b) {
	return a - b;
}
*/
//******************************************************
/*
class MathTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(MathTester);
	CPPUNIT_TEST(testAdd);
	CPPUNIT_TEST(testSub);
	CPPUNIT_TEST_SUITE_END();

public:
void setUp(void);
void tearDown(void);

protected:
void testAdd(void);
void testSub(void);

private:
MathClass *testObject;
};

void MathTester::testAdd(void) {
	CPPUNIT_ASSERT(5 == testObject->adder(2,3));
}

void MathTester::testSub(void) {
	CPPUNIT_ASSERT(2 == testObject->subber(5,3));
}

void MathTester::setUp(void) {
	testObject = new MathClass();
}

void MathTester::tearDown(void) {
	delete testObject;
}
*/
//***********************************************************

// Set test class here
CPPUNIT_TEST_SUITE_REGISTRATION(TestMaster);

int main(){
	CPPUNIT_NS::TestResult test_result;

	CPPUNIT_NS::TestResultCollector collector;
	test_result.addListener(&collector);

	CPPUNIT_NS::BriefTestProgressListener progress;
	test_result.addListener (&progress);

	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
	testrunner.run(test_result);

	CPPUNIT_NS::CompilerOutputter compileroutputter(&collector, std::cerr);
	compileroutputter.write ();

	return collector.wasSuccessful() ? 0 : 1;
}
