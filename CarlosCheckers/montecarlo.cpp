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
	return (node->win_count / static_cast<double>(node->sim_count)) + C * (sqrt(log(tsim_count) / static_cast<double>(node->sim_count)));
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
	if (node->has_goal) return WIN;
	else if (node->has_loss) return LOSS;
	int result;
	NodePtr temp;
	if (node->children.empty()){
		vector<Board> moves = Checkers::getLegalBoards(node->board, player);
		if (moves.size() == 0) return LOSS;
		Board move = moves.back();
		for (size_t i = 0; i < moves.size(); i++) {
			if (Checkers::goalTest(moves[i], player)) {
				move = moves[i];
				if (player == Checkers::getPlayer()) node->has_goal = true;
				else node->has_loss = true;
				break;
			}
		}
		if (!(node->has_goal || node->has_loss)) result = simulation(move, (player == 1) ? 2 : 1);		
		else result = INT_MAX;
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
			node->children.pop();
			result = search(temp, (player == 1) ? 2 : 1);
			temp->sim_count++;
			temp->win_count += result;
			temp->worth = evaluationUCB1(temp);
			node->children.push(temp);
		}
	}
		return result;
}

int MonteCarlo::simulation(Board board, short player){
	short me = Checkers::getPlayer();
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
