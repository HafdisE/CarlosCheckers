#include "montecarlo.h"

MonteCarlo::~MonteCarlo(){
	clearTree();
}

void MonteCarlo::clearTree(){
	clearTree(root);
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
	if (!node)(C * (sqrt(log(tsim_count))));
	return (node->win_count / (double)node->sim_count) + C * (sqrt(log(tsim_count) / (double)node->sim_count));
}

/*
CBmove2 MonteCarlo::search(){
	/* do something */
	/*return CBmove2();
}*/

int MonteCarlo::search(NodePtr node, short player){
	vector<Board> moves = Checkers::getLegalBoards(node->board, player);
	int result;
	if (node->children.empty()){
		result = simulation((player == 1) ? 2 : 1);
		node->children.push_back(new Node(1, result, moves.front()));
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
			result = simulation((player == 1) ? 2 : 1);
			node->children.push_back(new Node(1, result, moves[maxNode]));
		} else {
			result = search(node->children[maxNode], (player == 1) ? 2 : 1);
			node->sim_count++;
			node->win_count =+ result;
		}
	}
		return result;
}

int MonteCarlo::simulation(short player){
	srand((unsigned)time(0));
	Board currMove;
	stack<Board> pastMoves;
	int isGoal = 0;
	for (size_t i = 0; i < SIMULATION_LENGTH; i++){
		isGoal = Checkers::goalTest(Checkers::getBoard(), player);
		if (isGoal == WIN || isGoal == DRAW) break;
		vector<Board> moves = Checkers::getLegalBoards(currMove, player);
		currMove = moves[rand() % moves.size()];
		pastMoves.push(currMove);
		//Checkers::applyMove(currMove);
	}
	while (!pastMoves.empty()){
		//Checkers::undoMove(pastMoves.top());
		pastMoves.pop();
	}

	return isGoal;
}
