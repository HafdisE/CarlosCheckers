#ifndef NODE_H
#define NODE_H

#include <vector>
#include "checkers.h"
using namespace std;

struct Node
{
	Node(int sim_count, int win_count, CBmove2 move) : sim_count(sim_count), win_count(win_count), move(move), is_goal(false) { };
	int sim_count, win_count;
	bool is_goal;
	vector<Node*> children;
	CBmove2 move;
};

typedef Node* NodePtr;

#endif
