#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "montecarlo.h"

MonteCarlo::~MonteCarlo(){
	clearTree();
}

void MonteCarlo::clearTree(){
	clearTree(root);
	root = NULL;
}

void MonteCarlo::clearTree(NodePtr node){
	if (!node){
		for (unsigned int i = 0; i < node->children.size(); i++){
			clearTree(node->children[i]);
		}
		delete node;
	}
}

void MonteCarlo::selectNode(int node){
	NodePtr temp = root->children[node];
	root->children[node] = NULL;
	clearTree();
	root = temp;
}

double MonteCarlo::evaluationUCB1(NodePtr node){
	if (!node) return (C * (sqrt(log(tsim_count))));
	return (node->win_count / (double)node->sim_count) + C * (sqrt(log(tsim_count) / (double)node->sim_count));
}


Board MonteCarlo::search(double maxtime, int* playnow, char str[255]){
	double start = clock();
	//clearTree();
	root = new Node(0, 0, Checkers::getBoard());
	//temporary hack
	int calls = 0;
	while (true) {
		//sprintf(str, "Searching. Time is: %lf", (clock() - start));
		if (((clock() - start) >= maxtime*1000)) break;
		search(root, Checkers::getPlayer());
		calls++;
	}

	//sprintf(str, "Search complete with %d calls to search", calls);
	int index = 0;
	double max = INFMIN;
	double eval;
	for (size_t i = 0; i < root->children.size(); i++) {
		eval = evaluationUCB1(root->children[i]);
		if (eval > max) {
			max = eval;
			index = i;
		}
	}

	selectNode(index);
	
	return root->board;
}

int MonteCarlo::search(NodePtr node, short player){
	vector<Board> moves = Checkers::getLegalBoards(node->board, player);
	if (moves.size() == 0) return LOSS;
	int result;
	if (node->children.empty()){
		result = simulation(moves.front(), (player == 1) ? 2 : 1);
		node->children.push_back(new Node(1, result, moves.back()));
	} else {
		double maxValue = INFMIN;
		int maxNode = 0;
		for (size_t i = 0; i < node->children.size(); i++) {
			double currValue = evaluationUCB1(node->children[i]);
			if (currValue > maxValue){
				maxValue = currValue;
				maxNode = i;
			}
		}
		if ((moves.size() > node->children.size()) && (evaluationUCB1(NULL) > maxValue)){
			maxNode++;
			result = simulation(moves[maxNode], (player == 1) ? 2 : 1);
			node->children.push_back(new Node(1, result, moves[maxNode]));
		} else {
			result = search(node->children[maxNode], (player == 1) ? 2 : 1);
			node->sim_count++;
			node->win_count =+ result;
		}
	}
		return result;
}

int MonteCarlo::simulation(Board board, short player){
	short me = player;
	random_device  rand_dev;
	mt19937 generator(rand_dev());
	Board currMove = board;
	int isGoal = 0;
	for (size_t i = 0; i < SIMULATION_LENGTH; i++){
		isGoal = Checkers::goalTest(currMove, player);
		if (isGoal == WIN || isGoal == LOSS) break;
		vector<Board> moves = Checkers::getLegalBoards(currMove, player);
		if (moves.size() == 0) {
			isGoal = LOSS;
			break;
		}
		uniform_int_distribution<int> distr(0, moves.size() - 1);
		currMove = moves[distr(generator)];
		player = (player == 1) ? 2 : 1;
	}

	//To be more sophisticated later.
	if (me == player) return isGoal;
	else return !isGoal;
}
