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

		static short toCellID(Checkers& checkers, coord coord) {
			return checkers.toCellID(coord);
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

		TEST_METHOD(TestToCellid)
		{
			Checkers white(WHITE);

			coord cell(4,1);
			short res = CheckersTester::toCellID(white, cell);
			Assert::AreEqual((short)7, res);
			coord cell2(2, 5);
			res = CheckersTester::toCellID(white, cell2);
			Assert::AreEqual((short)22, res);
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
			State def;
			Assert::AreEqual((short)UNKNOWN, Checkers::goalTest(&def, WHITE));
			Assert::AreEqual((short)UNKNOWN, Checkers::goalTest(&def, BLACK));
		}

		TEST_METHOD(TestGoalTestBlackWin)
		{
			Board board(4095, 0, 0);
			State def(board);
			Assert::AreEqual((short)LOSS, Checkers::goalTest(&def, WHITE));
			Assert::AreEqual((short)WIN, Checkers::goalTest(&def, BLACK));
		}

		TEST_METHOD(TestGoalTestWhiteWin)
		{
			Board board(0, 4293918720, 0);
			State def(board);
			Assert::AreEqual((short)LOSS, Checkers::goalTest(&def, BLACK));
			Assert::AreEqual((short)WIN, Checkers::goalTest(&def, WHITE));
		}

		TEST_METHOD(TestGoalTestTieFiftyMovesNothingHappen)
		{
			State def;
			def.setMovesSinceLastCapture(50);
			Assert::AreEqual((short)DRAW, Checkers::goalTest(&def, BLACK));
			Assert::AreEqual((short)DRAW, Checkers::goalTest(&def, WHITE));
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
			State state;
			Assert::AreEqual((size_t)7, Checkers::getLegalMoves(&state, BLACK).size());
		}
		TEST_METHOD(TestWhenWhiteHasToKillBlackThreePieces)
		{
			Board b(0, 0, 0);
			b.setPiece(14, BLACK | MAN);
			b.setPiece(15, BLACK | MAN);
			b.setPiece(19, WHITE | MAN);
			State state(b);
			Assert::AreEqual((size_t)24576, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)262144, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t) 0, BoardTester::getKingbit(b));
			Assert::AreEqual((size_t)1, Checkers::getLegalMoves(&state, BLACK).size());
		}

		TEST_METHOD(TestSuperBasicMoveGoddamn)
		{
			Board b(0, 0, 0);
			b.setPiece(14, WHITE | MAN);
			b.setPiece(18, WHITE | MAN);
			State state(b);
			Assert::AreEqual((size_t)3, Checkers::getLegalMoves(&state, WHITE).size());
		}
		
		TEST_METHOD(TestWhenWhiteHasToKillBlackFullBoard)
		{
			Board b(20478, 4290908160, 0);
			State state(b);
			Assert::AreEqual((size_t)1, Checkers::getLegalMoves(&state, BLACK).size());
		}



		
	};
	
}