#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "montecarlo.h"

MonteCarlo::MonteCarlo() : tsim_count(0) { 
	root = NULL; 
	generator = mt19937(rand_dev()); 
	s = 0;
#if LOGGING
	mclog.setFile("montecarlo");
#endif
}


MonteCarlo::~MonteCarlo(){
	clearTree();
#if LOGGING
	mclog.log("Monte Carlo destructor", "Tree destroyed");
#endif
}

void MonteCarlo::clearTree(){
	clearTree(root);
	root = NULL;
}

void MonteCarlo::clearTree(NodePtr node){
	if (node){
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

void MonteCarlo::updateTree() {
	if (root) {
		Board curr = Checkers::getBoard();
		//find the node containing the current board
		for (size_t i = 0; i < root->children.size(); i++) {
			if (root->children[i]->board == curr) {
				//select it and return
#if LOGGING
				mclog.log("Update tree", "Found node with corresponding board in children. Node selected.");
#endif
				selectNode(i);
				return;
			}
		}
	}
	
#if LOGGING
	mclog.log("Update tree", "Failed to find board in children, clearing tree and changing root.");
#endif
	clearTree();
	tsim_count = 0;
	//if root is null or current board wasn't one of the children
	root = new Node(0, 0, Checkers::getBoard());
}


Board MonteCarlo::search(double maxtime, int* playnow, char str[255]){
	double start = clock();
	updateTree();
#if LOGGING
	char logstr[128];
	Board curr = Checkers::getBoard();
	sprintf(logstr, "Actual board state: %d whitebit: %d kingbit: %d", curr.blackbit, curr.whitebit, curr.kingbit);
	mclog.log("Search", logstr);
	sprintf(logstr, "Starting search with blackbit: %d whitebit: %d kingbit: %d", root->board.blackbit, root->board.whitebit, root->board.kingbit);
	mclog.log("Search", logstr);

#endif
#if LOGGING
	int calls = 0;
#endif
	while (true) {
		if (((clock() - start) >= maxtime*1000)) break;
		search(root, Checkers::getPlayer());
#if LOGGING
		calls++;
#endif
	}

#if LOGGING
	sprintf(logstr, "Search complete with %d calls to search over %lf", calls, (clock()-start));
	mclog.log("Search", logstr);
#endif




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
	
#if LOGGING
	sprintf(logstr, "Returning board with blackbit: %d whitebit: %d kingbit: %d", root->board.blackbit, root->board.whitebit, root->board.kingbit);
	mclog.log("Search", logstr);
#endif
	return root->board;
}

int MonteCarlo::search(NodePtr node, short player){
	vector<Board> moves = Checkers::getLegalBoards(node->board, player);
	if (moves.size() == 0) return LOSS;
	int result;
	if (node->children.empty()){
		result = simulation(moves.front(), (player == 1) ? 2 : 1);
		node->children.push_back(new Node(1, result, moves.back()));
		s++;
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
			s++;
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
		distr = uniform_int_distribution<int>(0, moves.size() - 1);
		currMove = moves[distr(generator)];
		player = (player == 1) ? 2 : 1;
	}

	//To be more sophisticated later.
	if (me == player) return isGoal;
	else return !isGoal;
}
