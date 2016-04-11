#include <iostream>

#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Test);

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
