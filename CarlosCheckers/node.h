#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;

struct Node
{
	Node(int sim_count, int win_count, string move) : sim_count(sim_count), win_count(win_count), move(move) { };
	int sim_count, win_count;
	vector<Node*> children;
	string move;
};

typedef Node* NodePtr;

#endif
