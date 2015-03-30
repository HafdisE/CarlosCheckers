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
#define LOGGING 1
#define INFMIN  -300000000
#define SIMULATION_LENGTH 50
static double C  = sqrt(2.5);
/* Uses the checkers checker board logic tools to generate moves and use them in its search */
class MonteCarlo {
public:
	MonteCarlo();
	~MonteCarlo();
	Board search(double maxtime, int* playnow, char str[255]);
	void clearTree();
	int size(){ return s; };


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
	
	
	unordered_map<Board, vector<Board>> transposition_table;
	unordered_map<Board, short> draw_check;
	unordered_map<Board, int> simulation_draw_check;

	random_device rand_dev;
	mt19937 generator;
	uniform_int_distribution<int> distr;
#if LOGGING
	::Logger mclog;
#endif

};

#endif
