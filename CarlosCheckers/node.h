#ifndef NODE_H
#define NODE_H

#include <vector>
#include "checkers.h"
using namespace std;

struct Node
{
	Node(int sim_count, int win_count, Board board) : sim_count(sim_count), win_count(win_count), board(board), is_goal(false) { };
	int sim_count, win_count;
	bool is_goal;
	vector<Node*> children;
	Board board;
};

typedef Node* NodePtr;

#endif
