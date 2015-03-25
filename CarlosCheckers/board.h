#include <string>

#define OCCUPIED 0
#define WHITE 1
#define BLACK 2
#define MAN 4
#define KING 8
#define FREE 16
#define CHANGECOLOR 3
#define MAXDEPTH 99
#define MAXMOVES 28

class Board {
public:
	Board() : blackbit(4095), whitebit(4293918720), kingbit(0) {};
	Board(short blackbit, short whitebit, short kingbit) : blackbit(blackbit), whitebit(whitebit), kingbit(kingbit) {};
	Board::Board(const Board& other) : blackbit(other.blackbit), whitebit(other.whitebit), kingbit(other.kingbit) {}
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

private:
	int kingbit;
	int whitebit;
	int blackbit;
};