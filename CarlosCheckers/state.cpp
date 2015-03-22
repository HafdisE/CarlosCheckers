#include "state.h"

State::State() {

}

Board State::getBoard() const {
	return board;
}

short State::getMovesSinceLastCapture() const {
	return moves_since_last_capture;
}