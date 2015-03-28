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
	if (node->is_goal) return 0;
	vector<Board> moves = Checkers::getLegalBoards(node->board, player);
	int result;
	if (node->children.empty()){
		if (Checkers::goalTest(moves.front(), player)){
			
		}
		result = simulation(moves.front(), (player == 1) ? 2 : 1);
		node->children.push_back(new Node(1, result, moves.back()));
		if (Checkers::goalTest(moves.front(), player) == WIN){
			node->children.front()->is_goal = true;
		}
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
			if (Checkers::goalTest(moves[maxNode], player) == WIN){
				node->children.front()->is_goal = true;
			}
		} else {
			result = search(node->children[maxNode], (player == 1) ? 2 : 1);
			node->sim_count++;
			node->win_count =+ result;
		}
	}
		return result;
}

int MonteCarlo::simulation(Board board, short player){
	srand((unsigned)time(0));
	Board currMove = board;
	int isGoal = 0;
	for (size_t i = 0; i < SIMULATION_LENGTH; i++){
		isGoal = Checkers::goalTest(Checkers::getBoard(), player);
		if (isGoal == WIN || isGoal == DRAW) break;
		vector<Board> moves = Checkers::getLegalBoards(currMove, player);
		currMove = moves[rand() % moves.size()];
	}

	return isGoal;
}
