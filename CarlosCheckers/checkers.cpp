#include "checkers.h"

/* board hashing */

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

void Checkers::updateState(CBmove2 move) {
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

short Checkers::goalTest(State* state, short player) {
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

	//TODO: THE OTHER DRAW CONDITION

	return UNKNOWN;
}

counter Checkers::countPieces(Board* board) {
	counter count;
	short piece;
	for (int i = 1; i <= 32; i++) {
		piece = board->getPiece(i);
		if (piece & WHITE) count.white++;
		if (piece & BLACK) count.black++;		
	}

	return count;
}

short Checkers::toCellID(coord co) {
	if (co.x == -1) return 0;
	return 0;
}

//TODO: incomplete
State Checkers::applyMove(State state, CBmove2 move) {
	int moves = state.getMovesSinceLastCapture();
	Board board = state.getBoard();
	if (move.delpiece[0] != FREE) {
		moves = 0;
		for (int i = 0; i < 12; i++) {
			if (move.del[i].x == -1) break;
			board.setPiece(toCellID(move.del[i]), FREE);
		}
	}
	else {
		moves++;
	}
	board.setPiece(toCellID(move.from), FREE);
	board.setPiece(toCellID(move.to), move.newpiece);
	
	State ret(board);
	ret.setMovesSinceLastCapture(moves);

	return state;
}



vector<CBmove2> Checkers::getLegalMoves(State* state, short player) {
	vector<CBmove2> normal, captures;
	vector<movp> path;
	Board board = state->getBoard(); 
	bool captured = false;
	for (int i = 1; i < 32; i++) {
		if(!(board.getPiece(i) & player)) continue;
		generateMoves(&board, board, i, player, &normal, &captures, &path, &captured);
	}
	return (captured ? captures : normal);
}

void Checkers::generateMoves(Board* original, Board board, short cell, short player, vector<CBmove2> *normal, vector<CBmove2> *capture, vector<movp>* path, bool *captured, int depth) {
	vector<movp> moves;
	if (depth == 0 || *(captured))  {
		moves = getCaptures(cell, &board);
		if (moves.size() > 0) *captured = true;
	}
	if (depth == 0 && !(*captured)) {
		moves = getMoves(cell, &board);
	}

	for (size_t i = 0; i < moves.size(); i++) {
		path->push_back(moves[i]);
		generateMoves(original, applySingleMove(board, moves[i]), moves[i].to, player, normal, capture, path, captured, depth + 1);
		path->pop_back();
	}

	//we can move no more
	if (moves.size() == 0) {
		//return if we didn't move at all
		if (path->size() == 0) return;
		//otherwise we make a move thingamadoodad
		struct CBmove2 new_move;
		//and copy the board's original state
		Board play = *original;
		new_move.oldpiece = play.getPiece((*path)[0].from);
		new_move.from = toCoord((*path)[0].from);
		bool caused_a_death = false;
		//path size shouldn't ever exceed 12
		for (size_t i = 0; i < path->size(); i++) {
			//now we play and write down all the deaths we caused

			//log intermediate moves
			if (i < path->size() - 1) new_move.path[i] = toCoord((*path)[i].to);
			//if there was a capture (if there's more than one move, there should always be one, otherwise this is wrong)
			if ((*path)[i].capture) {
				caused_a_death = true;
				new_move.delpiece[i] = play.getPiece((*path)[i].capture);
				new_move.del[i] = toCoord((*path)[i].capture);
			}
			//update the board
			play = applySingleMove(play, (*path)[i]);
		}
		new_move.to = toCoord((*path)[path->size() - 1].to);
		new_move.newpiece = play.getPiece((*path)[path->size() - 1].to);
		if (caused_a_death) {
			capture->push_back(new_move);
		}
		else {
			normal->push_back(new_move);
		}
	}
}

Board Checkers::applySingleMove(Board board, movp move) {
	short piece = board.getPiece(move.from);

	board.setPiece(move.from, FREE);

	if (move.capture) board.setPiece(move.capture, FREE);
	if (promotionCheck(move.to, piece)) piece |= KING;

	board.setPiece(move.to, piece);
	
	return board;
}

vector<short> Checkers::getDirectionsWhereType(short cell_id, Board *board, short type, bool north, bool south) {
	vector<short> directions;
	if (north) {
		if (!isLeftPiece(cell_id) && (board->getPiece(NW(cell_id)) & type) == type) {
			directions.push_back(NORTHWEST);
		}
		if (!isRightPiece(cell_id) && (board->getPiece(NE(cell_id)) & type) == type) {
			directions.push_back(NORTHEAST);
		}
	}
	if (south) {
		if (!isLeftPiece(cell_id) && (board->getPiece(SW(cell_id)) & type) == type) {
			directions.push_back(SOUTHWEST);
		}
		if (!isRightPiece(cell_id) && (board->getPiece(SE(cell_id)) & type) == type) {
			directions.push_back(SOUTHWEST);
		}
	}
	return directions;
}

bool Checkers::isLeftPiece(short cell_id) {
	return cell_id == 4 || cell_id == 12 ||
		cell_id == 20 || cell_id == 28;
}

bool Checkers::isRightPiece(short cell_id) {
	return 	cell_id == 5  || cell_id == 13 ||
		    cell_id == 21 || cell_id == 29;
}
vector<movp> Checkers::getCaptures(short cell_id, Board* board) {
	short piece;
	vector<short> directions;
	vector<movp> moves;
	bool north;
	bool south;
	short type;

	piece = board->getPiece(cell_id);
	north = south = false;

	if (piece == FREE) return moves; //what are you doin mate

	if (piece & KING) {
		north = true;
		south = true;
	}
	else if (piece & WHITE) {
		south = true;
		type = BLACK;
	}
	else {
		north = true;
		type = WHITE;
	}

	directions = getDirectionsWhereType(cell_id, board, type, north, south);

	for (size_t i = 0; i < directions.size(); i++) {
		switch (directions[i]) {
		case NORTHWEST:
			if (!isLeftPiece(NW(cell_id)) && board->getPiece(NW(NW(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, NW(NW(cell_id)), NW(cell_id)));
			}
			break;
		case NORTHEAST:
			if (!isRightPiece(NW(cell_id)) && board->getPiece(NE(NE(cell_id))) == FREE) {
				moves.push_back(movp(NE(NE(cell_id)), NE(cell_id)));
			}
			break;
		case SOUTHWEST:
			if (!isLeftPiece(NW(cell_id)) && board->getPiece(SW(SW(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, SW(SW(cell_id)), SW(cell_id)));
			}
			break;
		case SOUTHEAST:
			if (!isRightPiece(NW(cell_id)) && board->getPiece(SE(SE(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, SE(SE(cell_id)), SE(cell_id)));
			}
			break;
		}
	}

	return moves;
}

vector<movp> Checkers::getMoves(short cell_id, Board* board) {
	short piece;
	vector<short> directions;
	vector<movp> moves;
	bool north;
	bool south;

	piece = board->getPiece(cell_id);
	north = south = false;

	if (piece == FREE) return moves; //what are you doin mate

	if (piece & KING) {
		north = true;
		south = true;
	}
	else if (piece & WHITE) {
		south = true;
	}
	else {
		north = true;
	}

	directions = getDirectionsWhereType(cell_id, board, FREE, north, south);

	for (size_t i = 0; i < directions.size(); i++) {
		switch (directions[i]) {
		case NORTHWEST:
			moves.push_back(movp(cell_id, NW(cell_id)));
			break;
		case NORTHEAST:
			moves.push_back(movp(cell_id, NE(cell_id)));
			break;
		case SOUTHWEST:
			moves.push_back(movp(cell_id, SW(cell_id)));
			break;
		case SOUTHEAST:
			moves.push_back(movp(cell_id, SE(cell_id)));
			break;
		}
	}

	return moves;
}

/* TODO: compare with simple checkers code to see where the fuck 0,0 is */
coord Checkers::toCoord(short cell_id) {
	if (cell_id < 1 || cell_id > 32) return coord(-1, -1);

	static coord vals[] = { coord(6, 7), coord(4, 7), coord(2, 7), coord(0, 7),
		             coord(7, 6), coord(5, 6), coord(3, 6), coord(1, 6),
					 coord(6, 5), coord(4, 5), coord(2, 5), coord(0, 5),
					 coord(7, 4), coord(5, 4), coord(3, 4), coord(1, 4),
					 coord(6, 3), coord(4, 3), coord(2, 3), coord(0, 3),
					 coord(7, 2), coord(5, 2), coord(3, 2), coord(1, 2),
					 coord(6, 1), coord(4, 1), coord(2, 1), coord(0, 1),
					 coord(7, 0), coord(5, 0), coord(3, 0), coord(1, 0) };
	return vals[cell_id-1];
}


bool Checkers::promotionCheck(short cell_id, short piece) {
	return ((piece & MAN) && (((piece & WHITE) && cell_id < 5) || ((piece & BLACK) && cell_id > 28)));
}

CBmove2::CBmove2() {
	ismove = 1;
	newpiece = oldpiece = FREE;
	from = coord(-1,-1);
	to = coord(-1,-1);
	for (int i = 0; i < 12; i++) {
		path[i] = coord(-1, -1);
		del[i] = coord(-1, -1);
		delpiece[i] = FREE;
	}
}