#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\logger.h"
#include "..\CarlosCheckers\montecarlo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CarlosCheckersTests
{

	TEST_CLASS(TestLogger)
	{
	public:

		TEST_METHOD(TestLoggerConstructor)
		{
			//::Logger l);
			//l.log("TestLogger","what is THIS");
		}
		TEST_METHOD(TestSearch) {
			Checkers::setPlayer(WHITE);
			MonteCarlo mc;
			int playnow = 0;
			char str[125];
			mc.search(5000, &playnow);
			mc.size();
		}

		TEST_METHOD(TestTranspositionTable) {
			MonteCarlo mc;
			Board b;
			vector<Board> m1, m2;
			mc.getLegalBoards(b, WHITE, m1);
			mc.getLegalBoards(b, WHITE, m2);			

			for (int i = 0; i < 20000; i++) {
				mc.getLegalBoards(b, WHITE, m1);

			}
			//Assert::AreEqual(m1.size(), m2.size());
		}

	};
}