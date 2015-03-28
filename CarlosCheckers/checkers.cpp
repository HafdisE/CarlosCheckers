#include "checkers.h"

bool CBmove2::operator==(const CBmove2 &other) const {
	bool res = (from == other.from) && (to == other.to) && (newpiece == other.newpiece) && (oldpiece == other.oldpiece);
	for (int i = 0; i < 12; i++) {
		if (!res) break;
		res &= (path[i] == other.path[i]) && (del[i] == other.del[i]) && (delpiece[i] == other.delpiece[i]);
	}
	return res;
}

short Checkers::player = WHITE;
Board Checkers::current_board = Board();

Board Checkers::getBoard() {
	return current_board;
}

void Checkers::setBoard(int board[8][8]) {
	Board b(0, 0, 0);
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			b.setPiece(toCellID(coord(x, y)), board[x][y]);
		}
	}
	current_board = b;
}

void Checkers::setBoard(Board board) {
	current_board = board;
}

int Checkers::getPlayer() {
	return player;
}

void Checkers::setPlayer(short newplayer) {
	player = newplayer;
}
/*
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
}*/

short Checkers::goalTest(Board &board, short player) {
	counter c = countPieces(&board);
	if ((c.black == 0 && player == BLACK) || (c.white == 0 && player == WHITE)) {
		return LOSS;
	}

	if ((c.black == 0 && player == WHITE) || (c.white == 0 && player == BLACK)) {
		return WIN;
	}

	//MISSING DRAW THINGS

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
	return (3 - co.x / 2) + (co.y * 4) + 1;
}

void Checkers::applyMove(CBmove2 move) {
	if (move.delpiece[0] != FREE) {
		for (int i = 0; i < 12; i++) {
			if (move.del[i].x == -1) break;
			current_board.setPiece(toCellID(move.del[i]), FREE);
		}
	}
	current_board.setPiece(toCellID(move.from), FREE);
	current_board.setPiece(toCellID(move.to), move.newpiece);
}

void Checkers::undoMove(CBmove2 move) {
	if (move.delpiece[0] != FREE) {
		for (int i = 0; i < 12; i++) {
			if (move.del[i].x == -1) break;
			current_board.setPiece(toCellID(move.del[i]), move.delpiece[i]);
		}
	}
	current_board.setPiece(toCellID(move.to), FREE);
	current_board.setPiece(toCellID(move.from), move.newpiece);
}



vector<CBmove2> Checkers::getLegalMoves(short player) {
	vector<CBmove2> normal, captures;
	vector<movp> path;
	bool captured = false;
	for (int i = 1; i <= 32; i++) {
		if (!(current_board.getPiece(i) & player)) continue;
		generateMoves(current_board, i, player, &normal, &captures, &path, &captured);
	}
	return (captured ? captures : normal);
}

void Checkers::generateMoves(Board board, short cell, short player, vector<CBmove2> *normal, vector<CBmove2> *capture, vector<movp>* path, bool *captured, int depth) {
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
		generateMoves(applySingleMove(board, moves[i]), moves[i].to, player, normal, capture, path, captured, depth + 1);
		path->pop_back();
	}

	//we can move no more
	if (moves.size() == 0) {
		//return if we didn't move at all
		if (path->size() == 0) return;
		//otherwise we make a move thingamadoodad
		struct CBmove2 new_move;
		//and copy the board's original state
		Board play = current_board;
		new_move.oldpiece = play.getPiece((*path)[0].from);
		new_move.from = toCoord((*path)[0].from);
		bool caused_a_death = false;
		//path size shouldn't ever exceed 12
		for (size_t i = 0; i < path->size(); i++) {
			//now we play and write down all the deaths we caused

			//log intermediate moves
			if (i < path->size() - 1) new_move.path[i] = toCoord((*path)[i].to);
			//if there was a capture (if there's more than one move, there should always be one, otherwise this is wrong)
			if ((*path)[i].capture > 0) {
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
	if (promotionCheck(move.to, piece)) {
		piece &= ~MAN;
		piece |= KING;
	}

	board.setPiece(move.to, piece);

	return board;
}

vector<short> Checkers::getDirectionsWhereType(short cell_id, Board *board, short type, bool north, bool south) {
	vector<short> directions;
	if (north) {
		if (boundaryCheck(NW(cell_id)) && !isLeftPiece(cell_id) && (board->getPiece(NW(cell_id)) & type) == type) {
			directions.push_back(NORTHWEST);
		}
		if (boundaryCheck(NE(cell_id)) && !isRightPiece(cell_id) && (board->getPiece(NE(cell_id)) & type) == type) {
			directions.push_back(NORTHEAST);
		}
	}
	if (south) {
		if (boundaryCheck(SW(cell_id)) && !isLeftPiece(cell_id) && (board->getPiece(SW(cell_id)) & type) == type) {
			directions.push_back(SOUTHWEST);
		}
		if (boundaryCheck(SE(cell_id)) && !isRightPiece(cell_id) && (board->getPiece(SE(cell_id)) & type) == type) {
			directions.push_back(SOUTHEAST);
		}
	}
	return directions;
}

bool Checkers::boundaryCheck(short cell_id) {
	return (cell_id > 0 && cell_id < 33);
}

bool Checkers::isLeftPiece(short cell_id) {
	return cell_id == 4 || cell_id == 12 ||
		cell_id == 20 || cell_id == 28;
}

bool Checkers::isRightPiece(short cell_id) {
	return 	cell_id == 5 || cell_id == 13 ||
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

	if (piece & WHITE) {
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
			if (boundaryCheck(NW(NW(cell_id))) && !isLeftPiece(NW(cell_id)) && board->getPiece(NW(NW(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, NW(NW(cell_id)), NW(cell_id)));
			}
			break;
		case NORTHEAST:
			if (boundaryCheck(NE(NE(cell_id))) && !isRightPiece(NE(cell_id)) && board->getPiece(NE(NE(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, NE(NE(cell_id)), NE(cell_id)));
			}
			break;
		case SOUTHWEST:
			if (boundaryCheck(SW(SW(cell_id))) && !isLeftPiece(SW(cell_id)) && board->getPiece(SW(SW(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, SW(SW(cell_id)), SW(cell_id)));
			}
			break;
		case SOUTHEAST:
			if (boundaryCheck(SE(SE(cell_id))) && !isRightPiece(SE(cell_id)) && board->getPiece(SE(SE(cell_id))) == FREE) {
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
	static coord select[] = { coord(6, 0), coord(4, 0), coord(2, 0), coord(0, 0),
		coord(7, 1), coord(5, 1), coord(3, 1), coord(1, 1),
		coord(6, 2), coord(4, 2), coord(2, 2), coord(0, 2),
		coord(7, 3), coord(5, 3), coord(3, 3), coord(1, 3),
		coord(6, 4), coord(4, 4), coord(2, 4), coord(0, 4),
		coord(7, 5), coord(5, 5), coord(3, 5), coord(1, 5),
		coord(6, 6), coord(4, 6), coord(2, 6), coord(0, 6),
		coord(7, 7), coord(5, 7), coord(3, 7), coord(1, 7), };
	return select[cell_id - 1];
}


bool Checkers::promotionCheck(short cell_id, short piece) {
	return (((piece & WHITE) && (cell_id < 5)) || ((piece & BLACK) && (cell_id > 28)));
}

CBmove2::CBmove2() {
	newpiece = oldpiece = FREE;
	from = coord(-1, -1);
	to = coord(-1, -1);
	for (int i = 0; i < 12; i++) {
		path[i] = coord(-1, -1);
		del[i] = coord(-1, -1);
		delpiece[i] = FREE;
	}
}