#include "board.h"

std::size_t Board::hash() const {
	return ((kingbit >> 1)^(whitebit << 1))*17 + blackbit*33;
}


short Board::getPiece(short cell_id) {
	if (cell_id > 32 || cell_id < 1) return -1;
	short shift = cell_id - 1;
	int piece = ((kingbit >> shift) & 1) << 2;
	piece |= ((whitebit >> shift) & 1) << 1;
	piece |= (blackbit >> shift) & 1;
	switch (piece)
	{
	case 0:
		return FREE;
	case 1:
		return WHITE | MAN;
	case 3:
		return WHITE | KING;
	case 2:
		return BLACK | MAN;
	case 6:
		return BLACK | KING;
	default:
		return -1;
	}
}

void Board::setPiece(short cell_id, short type) {
	if (cell_id > 32 || cell_id < 1) return;
	short shift = cell_id - 1;
	int mask = 1 >> shift;
	switch (type)
	{
	case FREE:
		mask = ~mask;
		whitebit &= mask;
		blackbit &= mask;
		kingbit  &= mask;
		break;
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
	default:
		break;
	}
	return;
}