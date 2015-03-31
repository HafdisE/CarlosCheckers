#include "checkers.h"

short Checkers::player = WHITE;
Board Checkers::current_board = Board();

/* Returns the current board */
Board Checkers::getBoard() {
	return current_board;
}


/* Set the board with an 8 by 8 array */
void Checkers::setBoard(int board[8][8]) {
	Board b(0, 0, 0);
	coord c;
	for (int i = 1; i <= 32; i++) {
		c = toCoord(i);
		b.setPiece(i, board[c.x][c.y]);
	}
			
	current_board = b;
}

/* Set the board with a board... */
void Checkers::setBoard(Board board) {
	current_board = board;
}

short Checkers::getPlayer() {
	return player;
}

/* Update the player */
void Checkers::setPlayer(short newplayer) {
	player = newplayer;
}

/* Takes in a board and player and tells you whether said player has won or lost or neither */
short Checkers::goalTest(Board &board, short player) {
	if ((board.blackbit == 0 && player == BLACK) || (board.whitebit == 0 && player == WHITE)) {
		return LOSS;
	}

	if ((board.blackbit == 0 && player == WHITE) || (board.whitebit == 0 && player == BLACK)) {
		return WIN;
	}

	return UNKNOWN;
}

/* Takes in a board and returns a counter struct with the black piece and white piece count */
counter Checkers::countPieces(Board &board) {
	counter count;
	short piece;
	for (int i = 1; i <= 32; i++) {
		piece = board.getPiece(i);
		if (piece & WHITE) count.white++;
		if (piece & BLACK) count.black++;
	}

	return count;
}

int Checkers::count(Board &board) {
	int count = 0;
	unsigned int mixBoard = board.blackbit | board.whitebit | board.kingbit;
	while (mixBoard) {
		count += (mixBoard & 1);
		mixBoard >>= 1;
	}
	return count;
}

/* Takes in coordinates and returns cell ID. Not used. Possibly incorrect. */
short Checkers::toCellID(coord &co) {
	if (co.x == -1) return 0;
	return (3 - co.x / 2) + (co.y * 4) + 1;
}

/* Returns legal boards resulting from moves made by the given player on the given board */
vector<Board> Checkers::getLegalBoards(Board &board, short player) {
	vector<Board> normal, captures;
	bool captured = false;
	for (int i = 1; i <= 32; i++) {
		if (!(board.getPiece(i) & player)) continue;
		generateMoves(board, i, normal, captures, captured);
	}
	return (captured ? captures : normal);
}

void Checkers::generateMoves(Board &board,  short cell, vector<Board> &normal, vector<Board> &capture, bool &captured, bool promoted, int depth) {
	vector<movp> moves;
	if (!promoted) {
		if (depth == 0 || captured)  {
			moves = getCaptures(cell, board);
			if (moves.size() > 0) captured = true;
		}
		if (depth == 0 && !captured) {
			moves = getMoves(cell, board);
		}

		for (size_t i = 0; i < moves.size(); i++) {
			applySingleMove(board, moves[i]);
			generateMoves(board, moves[i].to, normal, capture, captured, moves[i].promotion, depth + 1);
			undoSingleMove(board, moves[i]);
		}
	}

	//we can move no more
	if (moves.size() == 0 && depth > 0) {
		if (captured) {
			capture.push_back(board);
		}
		else {
			normal.push_back(board);
		}
	}
}

void Checkers::applySingleMove(Board &board, movp &move) {
	short piece = board.getPiece(move.from);

	board.setPiece(move.from, FREE);

	if (move.capture) board.setPiece(move.capture, FREE);
	if (move.promotion) {
		piece &= ~MAN;
		piece |= KING;
	}

	board.setPiece(move.to, piece);
}

void Checkers::undoSingleMove(Board &board, movp &move) {
	short piece = board.getPiece(move.to);

	board.setPiece(move.to, FREE);

	if (move.capture) board.setPiece(move.capture, move.capture_piece);
	if (move.promotion) {
		piece |= MAN;
		piece &= ~KING;
	}

	board.setPiece(move.from, piece);
}

vector<short> Checkers::getDirectionsWhereType(short cell_id, Board &board, short type, bool north, bool south) {
	vector<short> directions;
	if (north) {
		if (boundaryCheck(NW(cell_id)) && !isLeftPiece(cell_id) && (board.getPiece(NW(cell_id)) & type) == type) {
			directions.push_back(NORTHWEST);
		}
		if (boundaryCheck(NE(cell_id)) && !isRightPiece(cell_id) && (board.getPiece(NE(cell_id)) & type) == type) {
			directions.push_back(NORTHEAST);
		}
	}
	if (south) {
		if (boundaryCheck(SW(cell_id)) && !isLeftPiece(cell_id) && (board.getPiece(SW(cell_id)) & type) == type) {
			directions.push_back(SOUTHWEST);
		}
		if (boundaryCheck(SE(cell_id)) && !isRightPiece(cell_id) && (board.getPiece(SE(cell_id)) & type) == type) {
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
vector<movp> Checkers::getCaptures(short cell_id, Board& board) {
	short piece;
	vector<short> directions;
	vector<movp> moves;
	bool north;
	bool south;
	short type;

	piece = board.getPiece(cell_id);
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
			if (boundaryCheck(NW(NW(cell_id))) && !isLeftPiece(NW(cell_id)) && board.getPiece(NW(NW(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, NW(NW(cell_id)), NW(cell_id), board.getPiece(NW(cell_id)), promotionCheck(NW(NW(cell_id)), piece)));
			}
			break;
		case NORTHEAST:
			if (boundaryCheck(NE(NE(cell_id))) && !isRightPiece(NE(cell_id)) && board.getPiece(NE(NE(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, NE(NE(cell_id)), NE(cell_id), board.getPiece(NE(cell_id)), promotionCheck(NE(NE(cell_id)), piece)));
			}
			break;
		case SOUTHWEST:
			if (boundaryCheck(SW(SW(cell_id))) && !isLeftPiece(SW(cell_id)) && board.getPiece(SW(SW(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, SW(SW(cell_id)), SW(cell_id), board.getPiece(SW(cell_id)), promotionCheck(SW(SW(cell_id)), piece)));
			}
			break;
		case SOUTHEAST:
			if (boundaryCheck(SE(SE(cell_id))) && !isRightPiece(SE(cell_id)) && board.getPiece(SE(SE(cell_id))) == FREE) {
				moves.push_back(movp(cell_id, SE(SE(cell_id)), SE(cell_id), board.getPiece(SE(cell_id)), promotionCheck(SE(SE(cell_id)), piece)));
			}
			break;
		}
	}

	return moves;
}

vector<movp> Checkers::getMoves(short cell_id, Board &board) {
	short piece;
	vector<short> directions;
	vector<movp> moves;
	bool north;
	bool south;

	piece = board.getPiece(cell_id);
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
			moves.push_back(movp(cell_id, NW(cell_id), 0, promotionCheck(NW(cell_id), piece)));
			break;
		case NORTHEAST:
			moves.push_back(movp(cell_id, NE(cell_id), 0, promotionCheck(NE(cell_id), piece)));
			break;
		case SOUTHWEST:
			moves.push_back(movp(cell_id, SW(cell_id), 0, promotionCheck(SW(cell_id), piece)));
			break;
		case SOUTHEAST:
			moves.push_back(movp(cell_id, SE(cell_id), 0, promotionCheck(SE(cell_id), piece)));
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
	return ((piece & MAN) && (((piece & WHITE) && (cell_id < 5)) || ((piece & BLACK) && (cell_id > 28))));
}