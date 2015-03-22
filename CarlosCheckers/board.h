#include <vector>

using namespace std;

/* board logic stuff */

/* structs */

struct Board {};

/* interface */

/* Returns a vector of all legal moves on the current board for the given player */
vector<struct CBmove> getlegalmoves(struct Board *board, int player);

/* Returns the result of making a given move */
Board *makemove(struct CBmove *move, struct Board *board);