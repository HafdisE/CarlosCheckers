#include <vector>
#include "state.h"

#define NW(x) (x + 5)
#define NE(x) (x + 4)
#define SW(x) (x - 4)
#define SE(x) (x - 5)

#define UP 0
#define DOWN 1

#define IS_OCCUPIED(x) (x ^ FREE)



using namespace std;

/* Checkers game logic */
struct counter {
	short white = 0;
	short black = 0;
};
/* last item is a CBmove that has ismove = 0. Returns all legal moves for given state. */
struct CBmove** getLegalMoves(State* state);

State applyMove(State state, struct CBmove *move);

/* only return diagonals that are free/occupied by enemy pieces and are legal for the piece */
vector<short> getDiagonals(short cell_id, Board* board);
/* returns true if the cell is free to move to or contains an enemy you can jump over */
bool free_or_jumpable(short moving_piece_id, short direction, short cell_id, Board* board);

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
};


