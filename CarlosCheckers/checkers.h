#include <vector>

using namespace std;

/* Checkers game logic */

//TODO: decide board structure
class Board {
public:
	Board();
	~Board();
};


/* last item is a CBmove that has ismove = 0. Returns all legal moves for given board state and player. */
struct CBmove** getLegalMoves(Board board, int player);
/* takes in a board and move and returns the resulting board */
Board applyMove(Board board, struct CBmove *move);
/* default starting board */
Board newBoard();

//this is our player the montecarlo search refers to for info on the current state and which player we are
//it should be updated based on messages from the engine
class Checkers {
public:
	Checkers(int player);
	~Checkers();
	void updateBoard(struct CBmove *move);
	void updateBoard(Board board);
	Board getBoard();
	int getPlayer();

private:
	int player; //WHITE or BLACK
	Board current_board;

};


