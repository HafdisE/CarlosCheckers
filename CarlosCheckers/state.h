#include "board.h"
#include <string>

class State {
public:
	State();
	Board getBoard() const;
	short getMovesSinceLastCapture() const;
	void setTie();
	bool getTie();
private:
	Board board;
	bool is_tie;
	short moves_since_last_capture;
};

namespace std {

	template <>
	struct hash<State>
	{
		std::size_t operator()(const State& k) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return k.getBoard().hash();
		}
	};

}