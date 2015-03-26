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
	return (node->win_count / (double)node->sim_count) + C * (sqrt(log(tsim_count) / (double)node->sim_count));
}

NodePtr MonteCarlo::expand(NodePtr node){
	return NULL;
}