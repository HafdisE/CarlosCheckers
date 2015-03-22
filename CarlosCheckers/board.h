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
	Board() : blackbit(4095), whitebit(4293918848), kingbit(0) {};
	Board::Board(const Board& other) : blackbit(other.blackbit), whitebit(other.whitebit), kingbit(other.kingbit) {}
	short getPiece(short cell_id);
	std::size_t hash() const;
	
	bool operator==(const Board &other) const
	{
		return (kingbit == other.kingbit && whitebit == other.whitebit && blackbit == other.blackbit);
	}

private:
	int kingbit;
	int whitebit;
	int blackbit;
};