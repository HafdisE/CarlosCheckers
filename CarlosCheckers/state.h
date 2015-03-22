#include "board.h"
#include <string>

class State {
public:
	State(){}
	State(const State& state) : board(state.board), is_tie(state.is_tie), moves_since_last_capture(state.moves_since_last_capture) {}
	State(Board board) : board(board){}
	Board getBoard() const;
	short getMovesSinceLastCapture() const;
	void setTie();
	bool getTie();

	bool operator==(const State &other) const
	{
		return (board == other.board);
	}

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
			return k.getBoard().hash();
		}
	};

}