#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <iostream>
#include <sstream>
#include "defines.h"

namespace CarlosCheckersTests {
	struct BoardTester;
}

class Board {
public:
	Board() : blackbit(4095), whitebit(4293918720), kingbit(0) {};
	Board(unsigned int blackbit, unsigned int whitebit, unsigned int kingbit) : blackbit(blackbit), whitebit(whitebit), kingbit(kingbit) {};
	Board(const Board& other) : blackbit(other.blackbit), whitebit(other.whitebit), kingbit(other.kingbit) {}
	/* get piece at cell_id. accepts ranges 1 <= x <= 32, anything outside that range has it return -1 */
	short getPiece(short cell_id);
	void setPiece(short cell_id, short type);
	/* couldn't be arsed to overload the hash function. Makes no difference either way tbh. 
	   Might make this static or switch to the proper hash function overloading.. */
	std::size_t hash() const;
	
	
	/* equals operator overload */
	bool operator==(const Board &other) const
	{
		return (kingbit == other.kingbit && whitebit == other.whitebit && blackbit == other.blackbit);
	}

	unsigned int kingbit;
	unsigned int whitebit;
	unsigned int blackbit;
	
	short bias = 0;

private:
	
	friend struct ::CarlosCheckersTests::BoardTester;
};

namespace std {

	template <>
	struct hash<Board>
	{
		std::size_t operator()(const Board& k) const
		{
			return k.hash();
		}
	};

}

std::string boardToString(Board& b);
std::string pieceToString(short piece);
Board flipLinesForDB(Board &b);

#endif