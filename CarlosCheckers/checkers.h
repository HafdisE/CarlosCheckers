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
/* last item is a CBmove that has ismove = 0. Returns all legal moves for given state. */
struct CBmove** getLegalMoves(State* state);

State applyMove(State state, struct CBmove *move);

short goalTest(State* state, short player);

counter countPieces(Board* board);

//this is our player the montecarlo search refers to for info on the current state and which player we are
//it should be updated based on messages from the engine
class Checkers {
public:
	Checkers(int player);
	void updateState(struct CBmove *move);
	void updateState(State state);
	Board getBoard() const;
	State getState() const;
	int getPlayer() const;

private:
	int player; //WHITE or BLACK
	State current_state;
	unordered_map<State, short> repeat_check;
	/* only return diagonals that are free/occupied by enemy pieces and are legal for the piece */
	vector<short> getDiagonals(short cell_id, Board* board);
	void getUp(short cell_id, Board* board, vector<short>& vec);
	void getDown(short cell_id, Board* board, vector<short>& vec);
	/* returns true if the cell is free... duh */
	bool is_free(short cell_id, Board* board);
	/* returns true if your piece can capture a piece in the given direction */
	bool jumpable(short moving_piece_id, short direction, short cell_id, Board* board);
	void tieCheck(short count, short new_count);
};


