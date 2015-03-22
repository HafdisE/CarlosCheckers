#include <vector>
#include "state.h"

using namespace std;

/* Checkers game logic */

/* last item is a CBmove that has ismove = 0. Returns all legal moves for given state. */
struct CBmove** getLegalMoves(State state);

State applyMove(State state, struct CBmove *move);

vector<short> getDiagonals(short cell_id, short piece);

//this is our player the montecarlo search refers to for info on the current state and which player we are
//it should be updated based on messages from the engine
class Checkers {
public:
	Checkers(int player);
	void updateState(struct CBmove *move);
	void updateState(State board);
	Board getBoard() const;
	State getState() const;
	int getPlayer() const;

private:
	int player; //WHITE or BLACK
	State current_state;
	


};


