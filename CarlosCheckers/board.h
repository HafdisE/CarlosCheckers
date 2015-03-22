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
	Board();
	short getPiece(short cell_id);
	std::size_t hash() const;
private:
	int kingbit;
	int whitebit;
	int blackbit;
};

namespace std {

	template <>
	struct hash<Board>
	{
		std::size_t operator()(const Board& k) const
		{
			using std::size_t;
			using std::hash;

			return k.hash();
		}
	};

}