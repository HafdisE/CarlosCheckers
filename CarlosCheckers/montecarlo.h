#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "checkers.h"
#include <cmath>
#include "node.h"
static double C  = sqrt(2);
/* Uses the checkers checker board logic tools to generate moves and use them in its search */
class MonteCarlo{
public:
	/* Default constructor */
	MonteCarlo() : tsim_count(0),  checkers(Checkers(1)){};
	/* Destructor */
	~MonteCarlo();
	/* Start search */
	NodePtr search();
	/* Clear the entire tree */
	void clearTree();
	
private:
	NodePtr root;
	Checkers checkers;
	double evaluationUCB1(NodePtr node);
	NodePtr search(NodePtr node, short player);
	NodePtr expand(NodePtr node);
	void simulation(NodePtr node);
	void selectNode(int node);
	void clearTree(NodePtr node);
	
	int tsim_count;
};

#endif
