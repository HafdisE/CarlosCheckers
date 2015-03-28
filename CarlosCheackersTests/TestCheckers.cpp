#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\checkers.h"
#include "TestBoard.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace CarlosCheckersTests
{
	struct CheckersTester
	{
		static coord toCoord(short cell_id) {
			return Checkers::toCoord(cell_id);
		}

		static short toCellID(coord coord) {
			return Checkers::toCellID(coord);
		}

		static counter countPieces(Board *board) {
			return Checkers::countPieces(board);
		}

		static bool promotionCheck(short cell_id, short piece) {
			return Checkers::promotionCheck(cell_id, piece);
		}

		static vector<movp> getCaptures(short cell_id, Board* board) {
			return Checkers::getCaptures(cell_id, board);
		}

		static vector<movp> getMoves(short cell_id, Board* board) {
			return Checkers::getMoves(cell_id, board);
		}

		static Board applySingleMove(Board board, movp move) {
			return Checkers::applySingleMove(board, move);
		}
	};

	TEST_CLASS(TestCheckers)
	{
	public:		

		TEST_METHOD(TestToCoord)
		{			
			short cell = 10;
			coord res = CheckersTester::toCoord(cell);
			Assert::AreEqual(4, res.x);
			Assert::AreEqual(2, res.y);
		}

		TEST_METHOD(TestToCellid)
		{
			coord cell(3,1);
			short res = CheckersTester::toCellID(cell);
			Assert::AreEqual((short)7, res);
			coord cell2(1, 5);
			res = CheckersTester::toCellID(cell2);
			Assert::AreEqual((short)24, res);
		}

		TEST_METHOD(TestCountPiecesDefaultBoard)
		{
			Board def;
			counter result = CheckersTester::countPieces(&def);
			Assert::AreEqual((short)12, result.white);
			Assert::AreEqual((short)12, result.black);			
		}

		TEST_METHOD(TestPromotionForBlack)
		{
			Board b(0,0,0);
			b.setPiece(3, BLACK | MAN);
			b.setPiece(30, BLACK | MAN);
			Assert::IsFalse(CheckersTester::promotionCheck(3, BLACK | MAN));
			Assert::IsTrue(CheckersTester::promotionCheck(30, BLACK | MAN));
		}

		TEST_METHOD(TestPromotionForWhite)
		{
			Board b(0, 0, 0);
			b.setPiece(3, WHITE | MAN);
			b.setPiece(30, WHITE | MAN);
			Assert::IsTrue(CheckersTester::promotionCheck(3, WHITE | MAN));
			Assert::IsFalse(CheckersTester::promotionCheck(30, WHITE | MAN));
		}

		TEST_METHOD(TestGoalTestDefaultState)
		{
			Board def;
			Assert::AreEqual((short)UNKNOWN, Checkers::goalTest(def, WHITE));
			Assert::AreEqual((short)UNKNOWN, Checkers::goalTest(def, BLACK));
		}

		TEST_METHOD(TestGoalTestBlackWin)
		{
			Board def(4095, 0, 0);
			Assert::AreEqual((short)LOSS, Checkers::goalTest(def, WHITE));
			Assert::AreEqual((short)WIN, Checkers::goalTest(def, BLACK));
		}

		TEST_METHOD(TestGoalTestWhiteWin)
		{
			Board def(0, 4293918720, 0);
			Assert::AreEqual((short)LOSS, Checkers::goalTest(def, BLACK));
			Assert::AreEqual((short)WIN, Checkers::goalTest(def, WHITE));
		}

		/*TEST_METHOD(TestGoalTestTieFiftyMovesNothingHappen)
		{
			State def;
			def.setMovesSinceLastCapture(50);
			Assert::AreEqual((short)DRAW, Checkers::goalTest(&def, BLACK));
			Assert::AreEqual((short)DRAW, Checkers::goalTest(&def, WHITE));
		}
		*/
		TEST_METHOD(TestGetCapturesDefaultBoard)
		{
			Board def;
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(12, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(11, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(10, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(9, &def).size());
		}

		TEST_METHOD(TestGetMovesDefaultBoard)
		{
			Board def;
			Assert::AreEqual((size_t)1, CheckersTester::getMoves(12, &def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(11, &def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(10, &def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(9, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(8, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(7, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(6, &def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(5, &def).size());
		}

		TEST_METHOD(TestApplyCBMoveBlackKillTwoWhite)
		{
			Board start(0, 0, 0);
			start.setPiece(11, BLACK | MAN);
			start.setPiece(15, WHITE | MAN);
			start.setPiece(23, WHITE | MAN);
			Board result(0, 0, 0);
			result.setPiece(27, BLACK | MAN);
			CBmove2 move;
			move.from = coord(2, 2);
			move.to = coord(2, 6);
			move.path[0] = coord(4, 4);
			move.oldpiece = BLACK | MAN;
			move.newpiece = BLACK | MAN;
			move.del[0] = coord(3, 3);
			move.del[1] = coord(3, 5);
			move.delpiece[0] = WHITE | MAN;
			move.delpiece[1] = WHITE | MAN;
			Checkers::setBoard(start);
			Checkers::applyMove(move);
			Assert::AreEqual(BoardTester::getBlackbit(result), BoardTester::getBlackbit(Checkers::getBoard()));
			Assert::AreEqual(BoardTester::getWhitebit(result), BoardTester::getWhitebit(Checkers::getBoard()));
			Checkers::undoMove(move);
			Assert::AreEqual(BoardTester::getBlackbit(start), BoardTester::getBlackbit(Checkers::getBoard()));
			Assert::AreEqual(BoardTester::getWhitebit(start), BoardTester::getWhitebit(Checkers::getBoard()));
		}

		TEST_METHOD(TestApplyCBMoveUndoCBmoveGetLegalBlackKillTwoWhite)
		{
			Board start(0, 0, 0);
			start.setPiece(11, BLACK | MAN);
			start.setPiece(15, WHITE | MAN);
			start.setPiece(23, WHITE | MAN);
			Board result(0, 0, 0);
			result.setPiece(27, BLACK | MAN);
			Checkers::setBoard(start);
			vector<CBmove2> moves = Checkers::getLegalMoves(BLACK);
			Assert::AreEqual((size_t) 1, moves.size());
			Checkers::applyMove(moves[0]);
			Assert::AreEqual(BoardTester::getBlackbit(result), BoardTester::getBlackbit(Checkers::getBoard()));
			Assert::AreEqual(BoardTester::getWhitebit(result), BoardTester::getWhitebit(Checkers::getBoard()));
			Checkers::undoMove(moves[0]);
			Assert::AreEqual(BoardTester::getBlackbit(start), BoardTester::getBlackbit(Checkers::getBoard()));
			Assert::AreEqual(BoardTester::getWhitebit(start), BoardTester::getWhitebit(Checkers::getBoard()));
		}

		TEST_METHOD(TestApplySingleMoveDefaultBoard)
		{
			Board start(1024, 0, 0);
			Board result(16384, 0, 0);
			movp mov(11, 15);
			Assert::AreEqual(BoardTester::getBlackbit(result), BoardTester::getBlackbit(CheckersTester::applySingleMove(start, mov)));
		}

		TEST_METHOD(TestCheckIfAnyLegalMovesDefaultBoard)
		{
			Checkers::setBoard(Board());
			Assert::AreEqual((size_t)7, Checkers::getLegalMoves(BLACK).size());
		}
		TEST_METHOD(TestWhenWhiteHasToKillBlackThreePieces)
		{
			Board b(0, 0, 0);
			b.setPiece(14, BLACK | MAN);
			b.setPiece(15, BLACK | MAN);
			b.setPiece(19, WHITE | MAN);
			Checkers::setBoard(b);
			Assert::AreEqual((size_t)24576, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)262144, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t) 0, BoardTester::getKingbit(b));
			Assert::AreEqual((size_t)1, Checkers::getLegalMoves(BLACK).size());
		}

		TEST_METHOD(TestSuperBasicMoveGoddamn)
		{
			Board b(0, 0, 0);
			b.setPiece(14, WHITE | MAN);
			b.setPiece(18, WHITE | MAN);
			Checkers::setBoard(b);
			Assert::AreEqual((size_t)3, Checkers::getLegalMoves(WHITE).size());
		}
		
		TEST_METHOD(TestWhenWhiteHasToKillBlackFullBoard)
		{
			Board b(20478, 4290908160, 0);
			Checkers::setBoard(b);
			Assert::AreEqual((size_t)20478, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)4290908160, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t)0, BoardTester::getKingbit(b));
			Assert::AreEqual((size_t)1, Checkers::getLegalMoves(BLACK).size());
		}



		
	};
	
}