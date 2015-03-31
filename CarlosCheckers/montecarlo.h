#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <cmath>
#include <cstdlib>
#include <time.h>
#include <utility>
#include <unordered_map>
#include <random>
#include <assert.h>
#include "checkers.h"
#include "node.h"
#include "logger.h"
#include "defines.h"
#include "dblookup.h"
#include "HashMap.h"

static double C  = sqrt(2);
/* Uses the checkers checker board logic tools to generate moves and use them in its search */
class MonteCarlo {
public:
	MonteCarlo();
	~MonteCarlo();
	Board search(double maxtime, int* playnow);
	void clearTree();
	int size(){ return s; };

	bool drawCheck();
	void initDB();
	char *str;
	//er að vera með stæla. Doesn't want to be tested.
	vector<Board> getLegalBoards(Board& board, short player);
private:
	NodePtr root;
	int tsim_count;
	short player;
	int s;
	double evaluationUCB1(NodePtr node);
	int search(NodePtr node, short player);
	int simulation(Board board, short player);
	void selectNode(int index);
	void clearTree(NodePtr node);
	void updateTree();
	void updateNode(NodePtr node, short player, short result);
	short dbLookUp(Board& b, short player);
	bool db_initialised = false;
	

	//draw things
	int moves_since_last_capture = 0;
	int last_count = 24;

	random_device rand_dev;
	mt19937 generator;
	uniform_int_distribution<int> distr;
#if LOGGING
	::Logger mclog;
#endif

};

#endif
