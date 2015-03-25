#ifndef STATE_H
#define STATE_H

#include "board.h"
#include <string>

class State {
public:
	State(){}
	State(const State& state) : board(state.board), is_tie(state.is_tie), moves_since_last_capture(state.moves_since_last_capture) {}
	State(Board board) : board(board){}
	/* Get the board. */
	Board getBoard() const;
	/* Get moves since last capture. Is changed via states generated from move applications. */
	short getMovesSinceLastCapture() const;
	void setMovesSinceLastCapture(int moves);

	/* will be removed soon */
	void setTie();
	bool getTie();

	/* equals operator overload */
	bool operator==(const State &other) const
	{
		return (board == other.board);
	}

private:
	Board board;
	bool is_tie;
	short moves_since_last_capture;
};

/* hash function overload */
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

#endif