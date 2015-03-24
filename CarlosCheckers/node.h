#include <vector>
using namespace std;

struct Node
{
	int sim_count, win_count;
	vector<Node*> children;
	string move;
};

typedef Node* NodePtr;
