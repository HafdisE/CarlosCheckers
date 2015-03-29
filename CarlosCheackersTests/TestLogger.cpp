#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\logger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CarlosCheckersTests
{
	

	TEST_CLASS(TestLogger)
	{
	public:

		TEST_METHOD(TestLoggerConstructor)
		{
			::Logger l("test");
			l.log("TestLogger","what is THIS");
		}
	};
}