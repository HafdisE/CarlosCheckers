#ifndef NODE_H
#define NODE_H

#include <vector>
#include <queue>
#include "checkers.h"
using namespace std;


struct Node
{
	struct compare
	{
		bool operator()(const Node* l, const Node* r)
		{
			return l->worth < r->worth;
		}
	};
	Node(int sim_count, int win_count, Board board, double worth, Node* parent) : sim_count(sim_count), win_count(win_count), board(board), worth(worth), parent(parent) { };
	int sim_count, win_count;
	double worth;
	vector<Board> moves_left;
	priority_queue<Node*, vector<Node*>, compare> children;
	Node* parent;
	Board board;
};

typedef Node* NodePtr;

#endif
