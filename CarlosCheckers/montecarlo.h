#include "checkers.h"
#include <cmath>
#include "node.h"
static double C  = sqrt(2);
/* Uses the checkers checker board logic tools to generate moves and use them in its search */
class MonteCarlo{
public:
	MonteCarlo(){ tsim_count = 0; root = NULL; }
	~MonteCarlo();
	void search();
	void clearTree();
	
private:
	NodePtr root;
	double evaluationUCB1(NodePtr node);
	void expand(NodePtr node);
	void simulation(NodePtr node);
	void clearTree(NodePtr node);
	
	int tsim_count;
};