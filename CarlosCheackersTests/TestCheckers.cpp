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

		static counter countPieces(Board &board) {
			return Checkers::countPieces(board);
		}

		static bool promotionCheck(short cell_id, short piece) {
			return Checkers::promotionCheck(cell_id, piece);
		}

		static vector<movp> getCaptures(short cell_id, Board &board) {
			return Checkers::getCaptures(cell_id, board);
		}

		static vector<movp> getMoves(short cell_id, Board &board) {
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

		TEST_METHOD(TestMANYLEGAL)
		{
			Board b;
			for (int i = 0; i < 2000; i++) {
				Checkers::getLegalBoards(b, WHITE);
			}
		}

		TEST_METHOD(TestToCellid)
		{
			coord cell(4,1);
			short res = CheckersTester::toCellID(cell);
			Assert::AreEqual((short)6, res);
			coord cell2(1, 5);
			res = CheckersTester::toCellID(cell2);
			Assert::AreEqual((short)24, res);
		}

		TEST_METHOD(TestCountPiecesDefaultBoard)
		{
			Board def;
			counter result = CheckersTester::countPieces(def);
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
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(12, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(11, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(10, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getCaptures(9, def).size());
		}

		TEST_METHOD(TestGetMovesDefaultBoard)
		{
			Board def;
			Assert::AreEqual((size_t)1, CheckersTester::getMoves(12, def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(11, def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(10, def).size());
			Assert::AreEqual((size_t)2, CheckersTester::getMoves(9, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(8, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(7, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(6, def).size());
			Assert::AreEqual((size_t)0, CheckersTester::getMoves(5, def).size());
		}

		/*TEST_METHOD(TestApplyCBMoveBlackKillTwoWhite)
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
		}*/

		TEST_METHOD(TestApplyCBMoveUndoCBmoveGetLegalBlackKillTwoWhite)
		{
			Board start(0, 0, 0);
			start.setPiece(11, BLACK | MAN);
			start.setPiece(15, WHITE | MAN);
			start.setPiece(23, WHITE | MAN);
			Board result(0, 0, 0);
			result.setPiece(27, BLACK | MAN);
			Checkers::setBoard(start);
			vector<Board> moves = Checkers::getLegalBoards(start, BLACK);
			Assert::AreEqual((size_t)1, moves.size());
			//Checkers::applyMove(moves[0]);
			Assert::AreEqual(BoardTester::getBlackbit(result), BoardTester::getBlackbit(moves[0]));
			Assert::AreEqual(BoardTester::getWhitebit(result), BoardTester::getWhitebit(moves[0]));
			//Checkers::undoMove(moves[0]);
			//Assert::AreEqual(BoardTester::getBlackbit(start), BoardTester::getBlackbit(Checkers::getBoard()));
			//Assert::AreEqual(BoardTester::getWhitebit(start), BoardTester::getWhitebit(Checkers::getBoard()));
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
			Assert::AreEqual((size_t)7, Checkers::getLegalBoards(Board(), BLACK).size());
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
			Assert::AreEqual((size_t)1, Checkers::getLegalBoards(b, BLACK).size());
		}

		TEST_METHOD(TestSuperBasicMoveGoddamn)
		{
			Board b(0, 0, 0);
			b.setPiece(14, WHITE | MAN);
			b.setPiece(18, WHITE | MAN);
			Checkers::setBoard(b);
			Assert::AreEqual((size_t)3, Checkers::getLegalBoards(b, WHITE).size());
		}
		
		TEST_METHOD(TestWhenWhiteHasToKillBlackFullBoard)
		{
			Board b(20478, 4290908160, 0);
			Checkers::setBoard(b);
			Assert::AreEqual((size_t)20478, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)4290908160, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t)0, BoardTester::getKingbit(b));
			Assert::AreEqual((size_t)1, Checkers::getLegalBoards(b, BLACK).size());
		}

		TEST_METHOD(TestWhenBlackHasToKillOneWhiteWithAKing)
		{
			Board b(20478, 2948730880, 0);
			Checkers::setBoard(b);
			vector<Board> moves = Checkers::getLegalBoards(b, BLACK);
			Assert::AreEqual((size_t)20478, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)2948730880, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t)0, BoardTester::getKingbit(b));
			/*Assert::IsTrue((coord(3, 7) == moves[0].to));
			Assert::IsTrue((coord(7, 7) == moves[1].to));
			Assert::IsTrue((coord(3, 3) == moves[0].from));
			Assert::IsTrue((coord(4, 4) == moves[0].del[0]));
			Assert::IsTrue((coord(4, 6) == moves[0].del[1]));
			Assert::IsTrue((coord(6, 6) == moves[1].del[1]));*/
			Assert::AreEqual((size_t)2, moves.size());
		}

		TEST_METHOD(TestTheBlackKing)
		{
			Board b(0, 0, 0);
			b.setPiece(18, BLACK | KING);
			b.setPiece(15, WHITE | MAN);
			b.setPiece(23, WHITE | MAN);
			Assert::AreEqual((size_t)131072, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)4210688, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t)131072, BoardTester::getKingbit(b));
			Assert::AreEqual((size_t)2, Checkers::getLegalBoards(b, BLACK).size());
		}

		TEST_METHOD(TestTheWhiteKingKillingTwoInRow)
		{
			Board b(16384, 151389696, 16384);
			Assert::AreEqual((size_t)16384, BoardTester::getBlackbit(b));
			Assert::AreEqual((size_t)151389696, BoardTester::getWhitebit(b));
			Assert::AreEqual((size_t)16384, BoardTester::getKingbit(b));
			vector<Board> moves = Checkers::getLegalBoards(b, BLACK);
			/*Assert::IsTrue((coord(3, 3) == moves[0].from));
			Assert::IsTrue((coord(1, 5) == moves[0].to));
			Assert::IsTrue((coord(7, 7) == moves[1].to));
			Assert::IsTrue((coord(5, 1) == moves[3].to));
			Assert::IsTrue((coord(1, 1) == moves[2].to));*/
			Assert::AreEqual((size_t)4, moves.size());
		}

		TEST_METHOD(TestWhiteMoveSixthBlackToSeventh) {
			//pls don't do this pls pls
			Board start(8391677, 2004353024, 0);
			Board wrong(134220797, 2004353024, 0);
			vector<Board> moves = Checkers::getLegalBoards(start, WHITE);
			for (size_t i = 0; i < moves.size(); i++) {
				Assert::IsFalse(wrong == moves[i]);
			}
		}
		
		TEST_METHOD(TestPromoteBlackPieceToKing)
			{
				Board start(0, 0, 0);
				start.setPiece(26, BLACK | MAN);
				Board resultOne(0, 0, 0);
				resultOne.setPiece(31, BLACK | KING);
				Board resultTwo(0, 0, 0);
				resultTwo.setPiece(30, BLACK | KING);
				Checkers::setBoard(start);
				vector<Board> moves = Checkers::getLegalBoards(start, BLACK);
				Assert::AreEqual((size_t)2, moves.size());
				Assert::AreEqual(BoardTester::getBlackbit(resultOne), BoardTester::getBlackbit(moves[0]));
				Assert::AreEqual(BoardTester::getBlackbit(resultTwo), BoardTester::getBlackbit(moves[1]));
				Assert::AreEqual(BoardTester::getKingbit(resultOne), BoardTester::getKingbit(moves[0]));
				Assert::AreEqual(BoardTester::getKingbit(resultTwo), BoardTester::getKingbit(moves[1]));
			}

		TEST_METHOD(TestPromoteBlackPieceToKingByKillingWhitePieceAndBlackKingStaysPut)
			{
				Board start(0, 0, 0);
				start.setPiece(23, BLACK | MAN);
				start.setPiece(25, WHITE | MAN);
				start.setPiece(26, WHITE | MAN);
				Board result(0, 0, 0);
				result.setPiece(30, BLACK | KING);
				result.setPiece(25, WHITE | MAN);
				Checkers::setBoard(start);
				vector<Board> moves = Checkers::getLegalBoards(start, BLACK);
				Assert::AreEqual((size_t)1, moves.size());
				Assert::AreEqual(BoardTester::getBlackbit(result), BoardTester::getBlackbit(moves[0]));
				Assert::AreEqual(BoardTester::getWhitebit(result), BoardTester::getWhitebit(moves[0]));
				Assert::AreEqual(BoardTester::getKingbit(result), BoardTester::getKingbit(moves[0]));
			}	
	};
	
}