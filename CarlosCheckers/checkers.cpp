#include "checkers.h"

/* board hashing */


Checkers::Checkers(int player) {
	this->player = player;
}

Board Checkers::getBoard() const {
	return current_state.getBoard();
}

State Checkers::getState() const {
	return current_state;
}

int Checkers::getPlayer() const {
	return player;
}

void Checkers::updateState(State state) {
	current_state = state;
}

void Checkers::updateState(struct CBmove *move) {
	current_state = applyMove(current_state, move);
}

State applyMove(State state, struct CBmove *move) {
	return state;
}

struct CBmove** getLegalMoves(State state) {
	return NULL;
}

vector<short> getDiagonals(short cell_id, short piece) {
	return vector<short>();
}