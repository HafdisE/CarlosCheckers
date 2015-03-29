#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <cmath>
#include <cstdlib>
#include <time.h>
#include <random>
#include "checkers.h"
#include "node.h"
#include "logger.h"
#define LOGGING 0
#define INFMIN  -300000000
#define SIMULATION_LENGTH 50
static double C  = sqrt(2);
/* Uses the checkers checker board logic tools to generate moves and use them in its search */
class MonteCarlo{
public:
	MonteCarlo();
	~MonteCarlo();
	Board search(double maxtime, int* playnow, char str[255]);
	void clearTree();
private:
	NodePtr root;
	double evaluationUCB1(NodePtr node);
	int search(NodePtr node, short player);
	int simulation(Board board, short player);
	void selectNode(int node);
	void clearTree(NodePtr node);
	void updateTree();
	
	int tsim_count;

	random_device rand_dev;
	mt19937 generator;
	uniform_int_distribution<int> distr;

#if LOGGING
	Logger mclog;
#endif

};

#endif
