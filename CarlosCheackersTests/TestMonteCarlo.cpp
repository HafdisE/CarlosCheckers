#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\montecarlo.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CarlosCheckersTests
{
	struct CarloTester
	{

	};

	TEST_CLASS(TestCarloSearch)
	{
	public:
		TEST_METHOD(TestSearch) {
			Checkers::setPlayer(WHITE);
			MonteCarlo mc;
			int playnow = 0;
			char str[125];
			mc.search(5, &playnow, str);
		}
			

	};
}