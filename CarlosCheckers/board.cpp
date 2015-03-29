#include "board.h"

std::size_t Board::hash() const {
	return ((kingbit >> 1)^(whitebit << 1))*17 + blackbit*33;
}


short Board::getPiece(short cell_id) {
	if (cell_id > 32 || cell_id < 1) return -1;
	short shift = cell_id - 1;
	unsigned int piece = ((kingbit >> shift) & 1) << 2;
	piece |= ((whitebit >> shift) & 1) << 1;
	piece |= (blackbit >> shift) & 1;
	switch (piece)
	{
	case 0:
		return FREE;
	case 2:
		return WHITE | MAN;
	case 6:
		return WHITE | KING;
	case 1:
		return BLACK | MAN;
	case 5:
		return BLACK | KING;
	default:
		return -1;
	}
}

void Board::setPiece(short cell_id, short type) {
	if (cell_id > 32 || cell_id < 1) return;
	short shift = cell_id - 1;
	int mask = 1 << shift;
	switch (type)
	{
	case WHITE|MAN:
		blackbit &= ~mask;
		whitebit |= mask;
		kingbit  &= ~mask;
		break;
	case WHITE|KING:
		blackbit &= ~mask;
		whitebit |= mask;
		kingbit  |= mask;
		break;
	case BLACK|MAN:
		whitebit &= ~mask;
		blackbit |= mask;
		kingbit &= ~mask;
		break;
	case BLACK|KING:
		whitebit &= ~mask;
		blackbit |= mask;
		kingbit |= mask;
		break;	
	case FREE:
	default:
		mask = ~mask;
		whitebit &= mask;
		blackbit &= mask;
		kingbit &= mask;
		break;
	}
	return;
}

std::string boardToString(Board& b) {
	std::stringstream ss(std::ios_base::app | std::ios_base::out);

	for (int i = 7; i >= 0; i--) {
		if (i % 2 == 1) {
			ss << "  ";
		}
		ss << pieceToString(b.getPiece(i * 4 + 4)) << "  ";
		ss << pieceToString(b.getPiece(i * 4 + 3)) << "  ";
		ss << pieceToString(b.getPiece(i * 4 + 2)) << "  ";
		ss << pieceToString(b.getPiece(i * 4 + 1));
		if (i % 2 == 0) {
			ss << "  ";
		}
		ss << "\n";
	}
	
	return ss.str();
}

std::string pieceToString(short piece) {
	std::stringstream ss;
	std::string ret;
	if (piece == FREE) ss << "  ";
	else ss << (piece & WHITE ? "W" : "B") << (piece & MAN ? "M" : "K");

	return ss.str();
}