#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\board.h"
#include "..\CarlosCheckers\board.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CarlosCheckersTests
{		
	TEST_CLASS(Board)
	{
	public:
		
		TEST_METHOD(BoardConstructor)
		{
			Board gameBoard;
			Assert::AreEqual(0, gameBoard.);
			// TODO: Your test code here
		}

	};
}