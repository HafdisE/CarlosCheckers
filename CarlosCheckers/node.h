#ifndef NODE_H
#define NODE_H

#include <vector>
#include "checkers.h"
using namespace std;

struct Node
{
	Node(int sim_count, int win_count, Board board, double worth) : sim_count(sim_count), win_count(win_count), board(board), worth(worth) { };
	int sim_count, win_count;
	double worth;
	vector<Node*> children;
	Board board;
};

typedef Node* NodePtr;

#endif
