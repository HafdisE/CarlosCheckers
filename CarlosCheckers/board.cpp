#include "board.h"

Board::Board() {
	blackbit = 4095;
	whitebit = 4293918848;
	kingbit = 0;
}

std::size_t Board::hash() const {
	return 1337;
}


short Board::getPiece(short cell_id) {
	if (cell_id > 32 || cell_id < 1) return -1;
	short shift = cell_id - 1;
	short piece = ((kingbit >> shift) & 1) << 2;
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