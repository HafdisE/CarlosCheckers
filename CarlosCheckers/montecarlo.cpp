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
		while (!node->children.empty()){
			clearTree(node->children.top());
			node->children.pop();
		}
		delete node;
		s--;
	}
}

void MonteCarlo::selectNode(){
	NodePtr temp = root->children.top();
	root->children.pop();
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
		while (!root->children.empty()) {
			
			if (root->children.top()->board == curr) {
				//select it and return
#if LOGGING
				mclog.log("Update tree", "Found node with corresponding board in children. Node selected.");
#endif
				selectNode();
				return;
			}
			NodePtr temp = root->children.top();
			root->children.pop();
			clearTree(temp);
		}
	}
	
#if LOGGING
	mclog.log("Update tree", "Failed to find board in children, clearing tree and changing root.");
#endif
	clearTree();
	tsim_count = 0;
	//if root is null or current board wasn't one of the children
	root = new Node(0, 0, Checkers::getBoard(), 0, NULL);
}


Board MonteCarlo::search(double maxtime, int* playnow, char str[255]){
	double start = clock();
	updateTree();
#if LOGGING
	stringstream ss;
	stringstream().swap(ss);
	ss << "Starting search as " << (Checkers::getPlayer() == WHITE ? "white" : "black") << endl << boardToString(root->board) << endl << root->board.blackbit << " " << root->board.whitebit << " " << root->board.kingbit;
	mclog.log("Search", ss.str());
	stringstream().swap(ss);
	int calls = 0;
#endif
	while (true) {
		if (((clock() - start) >= maxtime*1000)) break;
		search(root, Checkers::getPlayer());
		tsim_count++;
#if LOGGING
		calls++;
#endif
	}

#if LOGGING
	ss << "Search complete with " << calls << " calls to search over " << (clock() - start) / 1000 << "seconds";
	mclog.log("Search", ss.str());
	stringstream().swap(ss);
#endif


	selectNode();
	
#if LOGGING
	ss << "Returning board" << endl << boardToString(root->board) << endl << root->board.blackbit << " " << root->board.whitebit << " " << root->board.kingbit;
	mclog.log("Search", ss.str());
	stringstream().swap(ss);
#endif
	return root->board;
}


int MonteCarlo::search(NodePtr node, short player){
	int result;
	NodePtr temp;
	if (node->children.empty()){
		vector<Board> moves = Checkers::getLegalBoards(node->board, player);
		if (moves.size() == 0) return LOSS;
		result = simulation(moves.back(), (player == 1) ? 2 : 1);		
		temp = new Node(1, result, moves.back(), 0, node);
		moves.pop_back();
		temp->moves_left = moves;
		temp->worth = evaluationUCB1(temp);
		node->children.push(temp);
		s++;
	} else {
		if ((!node->moves_left.empty()) && (evaluationUCB1(NULL) > node->children.top()->worth)){
			result = simulation(node->moves_left.back(), (player == 1) ? 2 : 1);
			temp = new Node(1, result, node->moves_left.back(), 0, node);
			temp->worth = evaluationUCB1(temp);
			node->children.push(temp);
			node->moves_left.pop_back();
			s++;
		} else {
			temp = node->children.top();
			result = search(temp, (player == 1) ? 2 : 1);
			node->children.pop();
			node->children.push(temp);
			node->sim_count++;
			node->win_count += result;
			node->worth = evaluationUCB1(node);
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
