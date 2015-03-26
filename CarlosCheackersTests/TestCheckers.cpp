#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\checkers.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CarlosCheckersTests
{
	struct CheckersTester
	{
		static coord toCoord(Checkers& checkers, short cell_id) {
			return checkers.toCoord(cell_id);
		}

		static counter countPieces(Checkers& checkers, Board *board) {
			return checkers.countPieces(board);
		}

		static bool promotionCheck(Checkers& checkers, short cell_id, short piece) {
			return checkers.promotionCheck(cell_id, piece);
		}

		static vector<movp> getCaptures(Checkers& checkers, short cell_id, Board* board) {
			return checkers.getCaptures(cell_id, board);
		}

		static vector<movp> getMoves(Checkers& checkers, short cell_id, Board* board) {
			return checkers.getMoves(cell_id, board);
		}
	};

	TEST_CLASS(TestCheckers)
	{
	public:		

		TEST_METHOD(TestToCoord)
		{
			Checkers white(WHITE);
			
			short cell = 10;
			coord res = CheckersTester::toCoord(white, cell);
			Assert::AreEqual(4, res.x);
			Assert::AreEqual(5, res.y);
		}

		TEST_METHOD(TestCountPiecesDefaultBoard)
		{
			Checkers white(WHITE);
			Board def;
			counter result = CheckersTester::countPieces(white, &def);
			Assert::AreEqual((short)12, result.white);
			Assert::AreEqual((short)12, result.black);			
		}

		TEST_METHOD(TestPromotionForBlack)
		{
			Checkers white(WHITE);
			Board b(0,0,0);
			b.setPiece(3, BLACK | MAN);
			b.setPiece(30, BLACK | MAN);
			Assert::IsFalse(CheckersTester::promotionCheck(white, 3, BLACK | MAN));
			Assert::IsTrue(CheckersTester::promotionCheck(white, 30, BLACK | MAN));
		}

		TEST_METHOD(TestPromotionForWhite)
		{
			Checkers white(WHITE);
			Board b(0, 0, 0);
			b.setPiece(3, WHITE | MAN);
			b.setPiece(30, WHITE | MAN);
			Assert::IsTrue(CheckersTester::promotionCheck(white, 3, WHITE | MAN));
			Assert::IsFalse(CheckersTester::promotionCheck(white, 30, WHITE | MAN));
		}

		TEST_METHOD(TestGoalTestDefaultState)
		{
			Checkers white(WHITE);
			State def;
			Assert::AreEqual((short)UNKNOWN, white.goalTest(&def, WHITE));
			Assert::AreEqual((short)UNKNOWN, white.goalTest(&def, BLACK));
		}

		TEST_METHOD(TestGoalTestBlackWin)
		{
			Checkers white(WHITE);
			Board board(4095, 0, 0);
			State def(board);
			Assert::AreEqual((short)LOSS, white.goalTest(&def, WHITE));
			Assert::AreEqual((short)WIN, white.goalTest(&def, BLACK));
		}

		TEST_METHOD(TestGoalTestWhiteWin)
		{
			Checkers white(WHITE);
			Board board(0, 4293918720, 0);
			State def(board);
			Assert::AreEqual((short)LOSS, white.goalTest(&def, BLACK));
			Assert::AreEqual((short)WIN, white.goalTest(&def, WHITE));
		}

		TEST_METHOD(TestGoalTestTieFiftyMovesNothingHappen)
		{
			Checkers white(WHITE);
			State def;
			def.setMovesSinceLastCapture(50);
			Assert::AreEqual((short)DRAW, white.goalTest(&def, BLACK));
			Assert::AreEqual((short)DRAW, white.goalTest(&def, WHITE));
		}

		TEST_METHOD(TestGetCapturesDefaultBoard)
		{
			Checkers white(WHITE);
			Board def;
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(white, 12, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(white, 11, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(white, 10, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(white, 9, &def).size());
		}

		TEST_METHOD(TestGetMovesDefaultBoard)
		{
			Checkers white(WHITE);
			Board def;
			Assert::AreEqual((size_t)1, CheckersTester::getMoves(white, 12, &def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(white, 11, &def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(white, 10, &def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(white, 9, &def).size());
		}

		
	};
	
}