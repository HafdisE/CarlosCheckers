#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\checkers.h"
#include "TestBoard.cpp"


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

		static Board applySingleMove(Checkers& checkers, Board board, movp move) {
			return checkers.applySingleMove(board, move);
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
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(white, 8, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(white, 7, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(white, 6, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(white, 5, &def).size());
		}

		TEST_METHOD(TestApplySingleMoveDefaultBoard)
		{
			Checkers white(WHITE);
			Board start(1024, 0, 0);
			Board result(16384, 0, 0);
			movp mov(11, 15);
			Assert::AreEqual(BoardTester::getBlackbit(result), BoardTester::getBlackbit(CheckersTester::applySingleMove(white, start, mov)));
		}

		TEST_METHOD(TestCheckIfAnyLegalMovesDefaultBoard)
		{
			Checkers white(WHITE);
			State state;
			Assert::AreEqual((size_t)7, white.getLegalMoves(&state, BLACK).size());
		}
		TEST_METHOD(TestWhenWhiteHasToKillBlackThreePieces)
		{
			Board b(0, 0, 0);
			b.setPiece(14, WHITE | MAN);
			b.setPiece(15, WHITE | MAN);
			b.setPiece(19, BLACK | MAN);
			State state(b);
			Assert::AreEqual((size_t)24576, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t)262144, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t) 0, BoardTester::getKingbit(b));
			Assert::AreEqual((size_t)1, Checkers::getLegalMoves(&state, WHITE).size());
		}
		
		TEST_METHOD(TestWhenWhiteHasToKillBlackFullBoard)
		{
			Board b(4287184896, 16125, 0);
			State state(b);
			Assert::AreEqual((size_t)1, Checkers::getLegalMoves(&state, WHITE).size());
		}



		
	};
	
}