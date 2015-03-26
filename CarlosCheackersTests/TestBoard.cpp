#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CarlosCheckers\board.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CarlosCheckersTests
{
	struct BoardTester
	{
		static unsigned int getKingbit(Board& board) {
			return board.kingbit;
		}

		static void setKingbit(Board& board, int kb) {
			board.kingbit = kb;
		}

		static unsigned int getWhitebit(Board& board) {
			return board.whitebit;
		}

		static void setWhitebit(Board& board, int wb) {
			board.whitebit = wb;
		}
		static unsigned int getBlackbit(Board& board) {
			return board.blackbit;
		}

		static void setBlackbit(Board& board, int bb) {
			board.blackbit = bb;
		}
	};

	TEST_CLASS(TestBoard)
	{
	public:
		
		TEST_METHOD(BoardConstructor)
		{
			Board b;
			Assert::AreEqual((unsigned int)4095, BoardTester::getBlackbit(b));
			Assert::AreEqual((unsigned int)4293918720, BoardTester::getWhitebit(b));
			Assert::AreEqual((unsigned int)0, BoardTester::getKingbit(b));
		}

		TEST_METHOD(BoardGetPieceIllegal)
		{
			Board b;
			Assert::AreEqual((short)-1, b.getPiece(0));
			Assert::AreEqual((short)-1, b.getPiece(33));
		}

		TEST_METHOD(DefaultBoardGetPieceCellFirst)
		{
			Board b;
			Assert::AreEqual((short)(BLACK | MAN), b.getPiece(1));
		}

		TEST_METHOD(DefaultBoardGetPieceCellLast)
		{
			Board b;
			Assert::AreEqual((short)(WHITE | MAN), b.getPiece(32));
		}

		TEST_METHOD(DefaultBoardGetPieceMiddle)
		{
			Board b;
			Assert::AreEqual((short)FREE, b.getPiece(14));
		}

		TEST_METHOD(EmptyBoardSetBlackKingGetBlackKing)
		{
			Board b(0,0,0);
			b.setPiece(15, (BLACK | KING));
			Assert::AreEqual((short)(BLACK | KING), b.getPiece(15));
		}
		TEST_METHOD(EmptyBoardSetWhiteManGetWhiteMan)
		{
			Board b(0, 0, 0);
			b.setPiece(15, (WHITE | MAN));
			Assert::AreEqual((short)(WHITE | MAN), b.getPiece(15));
		}

		TEST_METHOD(FullBoardFreeCell)
		{
			Board b(~0, ~0, ~0);
			b.setPiece(15, FREE);
			Assert::AreEqual((short)FREE, b.getPiece(15));
		}
	};
}