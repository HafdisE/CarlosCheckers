#ifndef CHECKERS_H
#define CHECKERS_H

#include <vector>
#include <unordered_map>
//#include "state.h"
#include "board.h"
#include <utility>
#include "defines.h"

using namespace std;

namespace CarlosCheckersTests {
	struct CheckersTester;
}

struct movp {
	movp(short from, short to) : from(from), to(to) {}
	movp(short from, short to, short capture, short capture_piece) : from(from), to(to), capture(capture), capture_piece(capture_piece) {}
	movp(short from, short to, short capture, short capture_piece, bool promotion) : from(from), to(to), capture(capture), capture_piece(capture_piece), promotion(promotion) {}
	movp(short from, short to, short capture, bool promotion) : from(from), to(to), promotion(promotion) {}
	short to; //move to cell_id
	short from; //move from cell_id
	short capture = 0; //cell_id of captured piece if any (0 if not)
	short capture_piece = FREE;
	bool promotion = false;

	/* equals operator overload */
	bool operator==(const movp &other) const
	{
		return (to == other.to && from == other.from && capture == other.capture && promotion == other.promotion);
	}
};

/* this was bitching like CBmove so here it is...*/
struct coord {
	coord() : x(0), y(0) {}
	coord(int x, int y) : x(x), y(y) {}
	int x;
	int y;

	/* equals operator overload */
	bool operator==(const coord &other) const
	{
		return (x == other.x && y == other.y);
	}

};

/* A pair of sorts to return the count results */
struct counter {
	short white = 0;
	short black = 0;
};



//this is our player the montecarlo search refers to for info on the current state and which player we are
//it should be updated based on messages from the engine
class Checkers {
public:
	/* Update the current state using a state */
	//static void updateState(State state);
	/* Get pointer to the current board */
	static Board getBoard();
	static void setBoard(int board[8][8]);
	static void setBoard(Board board);
	/* Get pointer to the current state */
	//static State getState();
	/* Get the player */
	static short getPlayer();
	static void setPlayer(short player);

	

	/* Static public functions for use in conjunction with MCTS */

	/* Returns a vector of all the legal moves in the given state for the given player */
	static vector<Board> getLegalBoards(Board& board, short player);
	/* Returns WIN, LOSE, DRAW or UNKNOWN for the current state from the given player's POV */
	static short goalTest(Board& board, short player);

	static coord toCoord(short cell_id);
	static int count(Board& board);

private:
	static short player; //WHITE or BLACK
	static Board current_board;

	

	/* generates moves given a pointer to the board it is generated from, a copy of the board, the cell_id to generate moves from, the player's colour, a reference to
	an empty vector of moves without captures, and empty vector of  moves with captures, and empty vector containing a simplified move notation
	in order to keep track of the path of the move, and a boolean reference which lets it know whether a capture move has been detected or not */
	static void generateMoves(Board &board, short cell, vector<Board> &normal, vector<Board> &capture, bool &captures, bool promoted = false, int depth = 0);
	/* applies a 'single move', of the simplified move notation to the board and returns a new board */
	static void applySingleMove(Board &board, movp &move);
	static void undoSingleMove(Board &board, movp &move);
	/* returns directions in which there are cells containing type type from the cell at cell_id. Pass
	north and south boolean values to tell it whether it should check north and/or south */
	static vector<short> getDirectionsWhereType(short cell_id, Board &board, short type, bool north, bool south);
	/* checks if a piece has reached a promotion row and returns true if that is the case */
	static bool promotionCheck(short cell_id, short piece);
	static bool boundaryCheck(short cell_id);
	/* converts cell_id to coord */

	/* converts coord to cell_id (doesn't actually do this yet)*/
	static short toCellID(coord &co);
	/* gets all simplified notation moves for a single piece that is a capture move */
	static vector<movp> getCaptures(short cell_id, Board &board);
	/* gets all simplified notation moves for a single piece that are simple moves */
	static vector<movp> getMoves(short cell_id, Board &board);
	/* checks if a state has occurred three times, should be moved to montecarlo */
	static void tieCheck(short count, short new_count);
	/* helper function for goaltest, counts the pieces of both colours on the board */
	static counter countPieces(Board &board);

	static bool isLeftPiece(short cell_id);
	static bool isRightPiece(short cell_id);

	static friend struct ::CarlosCheckersTests::CheckersTester;
};

#endif


