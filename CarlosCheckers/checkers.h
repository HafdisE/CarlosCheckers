#include <vector>
#include <unordered_map>
#include "state.h"

#define NW(x) (x + 5)
#define NE(x) (x + 4)
#define SW(x) (x - 4)
#define SE(x) (x - 5)

#define UP 0
#define DOWN 1

/* return values */
#define DRAW 0
#define WIN 1
#define LOSS 2
#define UNKNOWN 3



using namespace std;

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
	void updateState(struct CBmove *move);
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
	static vector<CBmove*> getLegalMoves(State* state, short player);
	/* Returns a state which has the supplied move applied onto the supplied state */
	static State applyMove(State state, struct CBmove *move);
	/* Returns WIN, LOSE, DRAW or UNKNOWN for a given state from the given player's POV */
	static short goalTest(State* state, short player);	

private:
	short player; //WHITE or BLACK
	State current_state;
	/* TODO: move this to monte carlo things */
	unordered_map<State, short> repeat_check;
	/* only return diagonals that are free/occupied by enemy pieces and are legal for the piece */
	static void getDiagonals(short cell_id, Board* board, vector<short>& vec);
	static void getUp(short cell_id, Board* board, vector<short>& vec);
	static void getDown(short cell_id, Board* board, vector<short>& vec);
	/* returns true if the cell is free... duh */
	static bool is_free(short cell_id, Board* board);
	/* returns true if your piece can capture a piece in the given direction */
	static bool jumpable(short moving_piece_id, short direction, short cell_id, Board* board);
	void tieCheck(short count, short new_count);
	static counter countPieces(Board* board);
};


