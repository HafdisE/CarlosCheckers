#ifndef CHECKERS_H
#define CHECKERS_H

#include <vector>
#include <unordered_map>
//#include "state.h"
#include "board.h"
#include <utility>

#define NW(x) (!(((x-1)/4)%2) ? x + 5 : x + 4)
#define NE(x) (!(((x-1)/4)%2) ? x + 4 : x + 3)
#define SW(x) (!(((x-1)/4)%2) ? x - 3 : x - 4)
#define SE(x) (!(((x-1)/4)%2) ? x - 4 : x - 5)

#define NORTHWEST 0
#define NORTHEAST 1
#define SOUTHWEST 2
#define SOUTHEAST 3

/* return values */
#define DRAW 0
#define WIN 1
#define LOSS 2
#define UNKNOWN 3

using namespace std;

namespace CarlosCheckersTests {
	struct CheckersTester;
}

struct movp {
	movp(short from, short to) : from(from), to(to) {}
	movp(short from, short to, short capture) : from(from), to(to), capture(capture) {}
	short to; //move to cell_id
	short from; //move from cell_id
	short capture = 0; //cell_id of captured piece if any (0 if not)

	/* equals operator overload */
	bool operator==(const movp &other) const
	{
		return (to == other.to && from == other.from && capture == other.capture);
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

//the proper variety has no constructor and we shouldn't modify it so here this is
struct CBmove2            	/* all the information you need about a move */
{
	CBmove2();
	int newpiece;        /* what type of piece appears on to */
	int oldpiece;        /* what disappears on from */
	struct coord from, to; /* coordinates of the piece - in 8x8 notation!*/
	struct coord path[12]; /* intermediate path coordinates of the moving piece */
	struct coord del[12]; /* squares whose pieces are deleted after the move */
	int delpiece[12];    /* what is on these squares */

	/* equals operator overload */
	bool operator==(const CBmove2 &other) const;

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
	/* Update the current state using a move */
	static void applyMove(CBmove2 move);
	static void undoMove(CBmove2 move);
	/* Update the current state using a state */
	//static void updateState(State state);
	/* Get pointer to the current board */
	static Board getBoard();
	static void setBoard(Board board);
	/* Get pointer to the current state */
	//static State getState();
	/* Get the player */
	static int getPlayer();
	static void setPlayer(short player);

	/* Static public functions for use in conjunction with MCTS */

	/* Returns a vector of all the legal moves in the given state for the given player */
	static vector<CBmove2> getLegalMoves(short player);
	/* Returns WIN, LOSE, DRAW or UNKNOWN for the current state from the given player's POV */
	static short goalTest(Board& board, short player);

private:
	static short player; //WHITE or BLACK
	static Board current_board;
	//State current_state;
	/* TODO: move this to monte carlo things */
	//static unordered_map<Board, short> repeat_check;

	/* generates moves given a pointer to the board it is generated from, a copy of the board, the cell_id to generate moves from, the player's colour, a reference to
	   an empty vector of moves without captures, and empty vector of  moves with captures, and empty vector containing a simplified move notation
	   in order to keep track of the path of the move, and a boolean reference which lets it know whether a capture move has been detected or not */
	static void generateMoves(Board board, short cell, short player, vector<CBmove2> *normal, vector<CBmove2> *capture, vector<movp> *path, bool *captures, int depth = 0);
	/* applies a 'single move', of the simplified move notation to the board and returns a new board */
	static Board applySingleMove(Board board, movp move);
	/* returns directions in which there are cells containing type type from the cell at cell_id. Pass
	   north and south boolean values to tell it whether it should check north and/or south */
	static vector<short> getDirectionsWhereType(short cell_id, Board *board, short type, bool north, bool south);
	/* checks if a piece has reached a promotion row and returns true if that is the case */
	static bool promotionCheck(short cell_id, short piece);
	/* converts cell_id to coord */
	static coord toCoord(short cell_id);
	/* converts coord to cell_id (doesn't actually do this yet)*/
	static short toCellID(coord co);
	/* gets all simplified notation moves for a single piece that is a capture move */
	static vector<movp> getCaptures(short cell_id, Board* board);
	/* gets all simplified notation moves for a single piece that are simple moves */
	static vector<movp> getMoves(short cell_id, Board* board);
	/* checks if a state has occurred three times, should be moved to montecarlo */
	static void tieCheck(short count, short new_count);
	/* helper function for goaltest, counts the pieces of both colours on the board */
	static counter countPieces(Board* board);

	static bool isLeftPiece(short cell_id);
	static bool isRightPiece(short cell_id);

	static friend struct ::CarlosCheckersTests::CheckersTester;
};

#endif


