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
	short moves = current_state.getMovesSinceLastCapture();
	current_state = state;
	tieCheck(moves, current_state.getMovesSinceLastCapture());
}

void Checkers::updateState(struct CBmove *move) {
	short moves = current_state.getMovesSinceLastCapture();
	current_state = applyMove(current_state, move);
	tieCheck(moves, current_state.getMovesSinceLastCapture());
}

void Checkers::tieCheck(short count, short new_count) {
	if (count != new_count) {
		repeat_check.clear();
	}
	else {
		repeat_check[current_state]++;
		if (repeat_check[current_state] == 3) {
			current_state.setTie();
		}
	}
}

short goalTest(State* state, short player) {
	Board b = state->getBoard();
	counter c = countPieces(&b);
	if ((c.black == 0 && player == BLACK) || (c.white == 0 && player == WHITE)) {
		return LOSS;
	}

	if ((c.black == 0 && player == WHITE) || (c.white == 0 && player == BLACK)) {
		return WIN;
	}

	if (state->getMovesSinceLastCapture() >= 50) {
		return DRAW;
	}

	//TODO: THE OTHER DRAW CONDITIONS

	return UNKNOWN;
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

bool Checkers::jumpable(short me, short direction, short cell_id, Board* board) {
	short piece = board->getPiece(cell_id);
	if (piece == -1) return false; //illegal position
	if ((piece & WHITE) != (me & WHITE)) { //pieces aren't same type
		//so we can check if there is space behind them to jump to
		if (direction == UP) {
			if (is_free(NW(cell_id), board) || is_free(NE(cell_id), board)) return true;
		}
		else {
			if (is_free(SW(cell_id), board) || is_free(SE(cell_id), board)) return true;
		}
	}
	
	return false;
}

bool Checkers::is_free(short cell_id, Board* board) {
	short piece = board->getPiece(cell_id);
	if (piece == -1) return false; //illegal position
	return (piece ^ FREE);
}


void Checkers::getUp(short cell_id, Board* board, vector<short>& vec) {
	short piece = board->getPiece(cell_id);
	if (is_free(NW(cell_id), board) || jumpable(piece, UP, NW(cell_id), board)) {
		vec.push_back(NW(cell_id));
	}

	if (is_free(NE(cell_id), board) || jumpable(piece, UP, NE(cell_id), board)) {
		vec.push_back(NE(cell_id));
	}
}

void Checkers::getDown(short cell_id, Board* board, vector<short>& vec) {
	short piece = board->getPiece(cell_id);
	if (is_free(SW(cell_id), board) || jumpable(piece, DOWN, SW(cell_id), board)) {
		vec.push_back(SW(cell_id));
	}

	if (is_free(SE(cell_id), board) || jumpable(piece, DOWN, SE(cell_id), board)) {
		vec.push_back(SE(cell_id));
	}
}

vector<short> Checkers::getDiagonals(short cell_id, Board* board) {
	short piece = board->getPiece(cell_id);
	vector<short> diagonals;
	if (is_free(cell_id, board)) return diagonals; //wasn't occupied. the fuck are you doing.	
	if (piece & WHITE) {
		if (piece & KING) getUp(cell_id, board, diagonals);
		getDown(cell_id, board, diagonals);
	}
	else {
		if (piece & KING) getDown(cell_id, board, diagonals);
		getUp(cell_id, board, diagonals);
	}
	return diagonals;
}