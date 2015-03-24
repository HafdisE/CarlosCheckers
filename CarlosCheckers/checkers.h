#include <vector>
#include <unordered_map>
#include "state.h"
#include <utility>

#define NW(x) (x + 5)
#define NE(x) (x + 4)
#define SW(x) (x - 4)
#define SE(x) (x - 5)

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

struct movp {
	movp(short from, short to) : from(from), to(to) {}
	movp(short from, short to, short capture) : from(from), to(to), capture(capture) {}
	short to;
	short from;
	short capture = 0;
};

/* this was bitching like CBmove so here it is...*/
struct coord {
	coord(){ x = 0; y = 0; }
	coord(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};

//the proper variety has no constructor and we shouldn't modify it so here this is
struct CBmove2            	/* all the information you need about a move */
{
	CBmove2() {}
	int ismove = 1;          /* kind of superfluous: is 0 if the move is not a valid move */
	int newpiece;        /* what type of piece appears on to */
	int oldpiece;        /* what disappears on from */
	struct coord from, to; /* coordinates of the piece - in 8x8 notation!*/
	struct coord path[12]; /* intermediate path coordinates of the moving piece */
	struct coord del[12]; /* squares whose pieces are deleted after the move */
	int delpiece[12];    /* what is on these squares */
};

/* Checkers game logic */
struct counter {
	short white = 0;
	short black = 0;
};



//this is our player the montecarlo search refers to for info on the current state and which player we are
//it should be updated based on messages from the engine
class Checkers {
public:
	/*Constructor. Initialise game state tracker for player player.*/
	Checkers(short player) : player(player) {}
	/* Update the current state using a move */
	void updateState(CBmove2 move);
	/* Update the current state using a state */
	void updateState(State state);
	/* Get the current board */
	Board getBoard() const;
	/* Get the current state */
	State getState() const;
	/* Get the player */
	int getPlayer() const;

	/* Static public functions for use in conjunction with MCTS */

	/* Returns a vector of all the legal moves in the given state for the given player */
	static vector<CBmove2> getLegalMoves(State* state, short player);
	/* Returns a state which has the supplied move applied onto the supplied state */
	static State applyMove(State state, struct CBmove2 move);
	/* Returns WIN, LOSE, DRAW or UNKNOWN for a given state from the given player's POV */
	static short goalTest(State* state, short player);	

private:
	short player; //WHITE or BLACK
	State current_state;
	/* TODO: move this to monte carlo things */
	unordered_map<State, short> repeat_check;

	static void generateMoves(Board* original, Board board, short cell, short player, vector<CBmove2> *normal, vector<CBmove2> *capture, vector<movp> *path, bool *captures);
	static Board applySingleMove(Board board, movp move);
	static vector<short> getDirectionsWhereType(short cell_id, Board *board, short type, bool north, bool south);
	static bool promotionCheck(short cell_id, short piece);
	static coord toCoord(short cell_id);
	static vector<movp> getCaptures(short cell_id, Board* board);
	static vector<movp> getMoves(short cell_id, Board* board);
	void tieCheck(short count, short new_count);
	static counter countPieces(Board* board);
};


