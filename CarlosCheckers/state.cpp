#include "state.h"

Board State::getBoard() const {
	return board;
}

void State::setTie() {
	is_tie = true;
}

bool State::getTie() {
	return is_tie;
}



short State::getMovesSinceLastCapture() const {
	return moves_since_last_capture;
}