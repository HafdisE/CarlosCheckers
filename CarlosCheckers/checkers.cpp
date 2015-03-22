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

counter countPieces(Board* board) {
	counter count;
	short piece;
	for (int i = 1; i <= 32; i++) {
		piece = board->getPiece(i);
		if (piece & WHITE) count.white++;
		if (piece & BLACK) count.black++;		
	}

	return count;
}

State applyMove(State state, struct CBmove *move) {
	return state;
}

struct CBmove** getLegalMoves(State* state) {
	return NULL;
}

bool free_or_jumpable(short moving_piece_id, short direction, short cell_id,  Board* board) {
	short piece = board->getPiece(cell_id);
	short me = board->getPiece(moving_piece_id);
	if (piece == -1) return false; //illegal position
	if (!IS_OCCUPIED(piece)) return true; //free cell
	if ((piece & WHITE) != (me & WHITE)) { //pieces aren't same type
		//so we can check if there is space behind them to jump to
		if (direction == UP) {
			if (!IS_OCCUPIED(board->getPiece(NW(cell_id))) || !IS_OCCUPIED(board->getPiece(NE(cell_id)))) return true;
		}
		else {
			if (!IS_OCCUPIED(board->getPiece(SW(cell_id))) || !IS_OCCUPIED(board->getPiece(SE(cell_id)))) return true;
		}
		
	}
	
	return false;
}

/* this could be shortened */
vector<short> getDiagonals(short cell_id, Board* board) {
	short piece = board->getPiece(cell_id);
	vector<short> diagonals;
	if (!IS_OCCUPIED(piece)) return diagonals; //wasn't occupied. the fuck are you doing.
	if (piece & KING) {
		if (piece & WHITE) {
			if (free_or_jumpable(cell_id, UP, NW(cell_id), board)) {
				diagonals.push_back(NW(cell_id));
			}

			if (free_or_jumpable(cell_id, UP, NE(cell_id), board)) {
				diagonals.push_back(NE(cell_id));
			}

		}
		else {
			if (free_or_jumpable(cell_id, DOWN, SW(cell_id), board)) {
				diagonals.push_back(SW(cell_id));
			}

			if (free_or_jumpable(cell_id, DOWN, SE(cell_id), board)) {
				diagonals.push_back(SE(cell_id));
			}
		}
	}

	if (piece & BLACK) {
		if (free_or_jumpable(cell_id, UP, NW(cell_id), board)) {
			diagonals.push_back(NW(cell_id));
		}

		if (free_or_jumpable(cell_id, UP, NE(cell_id), board)) {
			diagonals.push_back(NE(cell_id));
		}

	}
	else {
		if (free_or_jumpable(cell_id, DOWN, SW(cell_id), board)) {
			diagonals.push_back(SW(cell_id));
		}

		if (free_or_jumpable(cell_id, DOWN, SE(cell_id), board)) {
			diagonals.push_back(SE(cell_id));
		}
	}

	return diagonals;
}