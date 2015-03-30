#ifndef NODE_H
#define NODE_H

#include <vector>
#include <queue>
#include "checkers.h"
using namespace std;


struct Node
{
	Node(int sim_count, int win_count, Board board, double worth) : sim_count(sim_count), win_count(win_count), board(board), has_goal(false), has_loss(false) { };
	int sim_count, win_count;
	vector<Node*> children;
	Board board;
	bool has_goal, has_loss;
};

typedef Node* NodePtr;

#endif
