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
		//Checkers::applyMove(moves.front());
		result = simulation();
		node->children.push_back(new Node(1, result, moves.front()));
		//Checkers::undoMove(moves.front());
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
			//Checkers::applyMove(node->children[maxNode]->move);
			result = simulation();
			node->children.push_back(new Node(1, result, moves[maxNode]));
		} else {
			//Checkers::applyMove((node->children[maxNode]->move));
			result = search(node->children[maxNode], (player == 1) ? 2 : 1);
			node->sim_count++;
			node->win_count =+ result;
		}
		//Checkers::undoMove(node->children[maxNode]->move);
	}
		return result;
}

int MonteCarlo::simulation(){
	return 0;
}
