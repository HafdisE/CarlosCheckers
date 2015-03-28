#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <cmath>
#include <cstdlib>
#include <time.h>
#include "checkers.h"
#include "node.h"
#define INFMIN  -300000000;
static double C  = sqrt(2);
/* Uses the checkers checker board logic tools to generate moves and use them in its search */
class MonteCarlo{
public:
	MonteCarlo() : tsim_count(0) { root = new Node(0, 0, CBmove2());};
	~MonteCarlo();
	CBmove2 search();
	void clearTree();
private:
	NodePtr root;
	double evaluationUCB1(NodePtr node);
	int search(NodePtr node, short player);
	int simulation();
	void selectNode(int node);
	void clearTree(NodePtr node);
	
	int tsim_count;
};

#endif
