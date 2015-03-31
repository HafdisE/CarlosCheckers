#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "montecarlo.h"

/* *	Default Constructor,  Sets the simulation count to 0, starts the random number generator and preps the logger */
MonteCarlo::MonteCarlo() : tsim_count(0){
	root = NULL;
	generator = mt19937(rand_dev());
	s = 0;
#if LOGGING
	mclog.setFile("montecarlo");
#endif
}

/* Destructor.  */

MonteCarlo::~MonteCarlo(){
	clearTree();
#if LOGGING
	mclog.log("Monte Carlo destructor", "Tree destroyed");
#endif
}

/* Clears the the tree, freeing all memory in it*/
void MonteCarlo::clearTree(){
	clearTree(root);
	root = NULL;
}

short MonteCarlo::dbLookUp(Board &board, short player, int conditional) {
	POSITION p;
	p.bk = board.blackbit & board.kingbit;
	p.bm = board.blackbit ^ board.kingbit;
	p.wk = board.whitebit & board.kingbit;
	p.wm = board.whitebit ^ board.kingbit;
	p.color = player;
	return dblookup(&p, conditional);;
}

/* Clear tree helper function */
void MonteCarlo::clearTree(NodePtr node){
	if (node){
		for (size_t i = 0; i < node->children.size(); i++){
			clearTree(node->children[i]);
		}
		delete node;
		s--;
	}
}


bool MonteCarlo::drawCheck() {
	int count = Checkers::count(Checkers::getBoard());
	if (count != last_count) {
		moves_since_last_capture = 0;
		last_count = count;
	}
	return moves_since_last_capture >= 50;
}

/* Blanks out the given node to save it from completion, calls clear tree on whats left then resets the root of the tree as the blanked out node*/
void MonteCarlo::selectNode(int index){
	NodePtr temp = root->children[index];
	root->children[index] = NULL;
	clearTree();
	root = temp;
	tsim_count = root->sim_count;
}

/* Just a UCB evaluation function */
double MonteCarlo::evaluationUCB1(NodePtr node){
	if (!node) return (C * (sqrt(log(tsim_count))));
	return (node->win_count / static_cast<double>(node->sim_count)) + static_cast<double>(C * (sqrt(log(tsim_count) / static_cast<double>(node->sim_count))));
}

/* Finds the given move in the opponents list and updates the tree to that move */
void MonteCarlo::updateTree(){
	if (root){
		Board curr = Checkers::getBoard();
		//find the node containing the current board
		for (size_t i = 0; i < root->children.size(); i++){
			if (root->children[i]->board == curr){
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


/* The base search function.  Calls the helper function while it has time to construct the search tree */
Board MonteCarlo::search(double maxtime, int* playnow){
	/* We start of by setting the clock and updating the tree based on the opponents move*/
	double start = clock();
	updateTree();

	/* Logging header for each search set up */
#if LOGGING
	stringstream ss;
	ss << "Starting search as " << (Checkers::getPlayer() == WHITE ? "white" : "black") << endl << boardToString(root->board) << endl << root->board.blackbit << " " << root->board.whitebit << " " << root->board.kingbit;
	mclog.log("Search", ss.str());
	stringstream().swap(ss);
#endif

	/* The basic search loop, checks the time, searches and increments tsim_count for use in the UCB */
	while (true){
		if (((clock() - start) >= maxtime * 1000)) break;
		search(root, Checkers::getPlayer());
		tsim_count++;
	}

	/* Logging search results for each search set up */
#if LOGGING
	ss << "Search complete with " << tsim_count << " simulations over " << (clock() - start) / 1000 << " sec. Tree is of size " << size() << ".";
	mclog.log("Search", ss.str());
	stringstream().swap(ss);
#endif

	/* We find the best node in the tree and pick that */
	size_t i;
	double maxValue = -1, currValue;
	for (i = 0; i < root->children.size(); i++){
		if ((currValue = evaluationUCB1(root->children[i])) > maxValue){
			maxValue = currValue;
		}
	}

	/*We pick the best node we found */
	selectNode(i - 1);

	/* Logging footer for each search*/
#if LOGGING
	ss << "Returning board" << endl << boardToString(root->board) << endl << root->board.blackbit << " " << root->board.whitebit << " " << root->board.kingbit;
	mclog.log("Search", ss.str());
	stringstream().swap(ss);
#endif
	/* We return the root board */

	sprintf(str, "size: %d sims: %d", size(), tsim_count);

	return root->board;
}

vector<Board> MonteCarlo::getLegalBoards(Board& board, short player){
//	if (transposition_table.contains(board)) {
//		transposition_table.put(Checkers::getLegalBoards(board, player), board);
	//}
//	vector<Board> moves;
//	transposition_table.get(board, moves);
	return Checkers::getLegalBoards(board, player);
}

/* The search function takes in a node and a player and recursively moves down the search tree until it hits a unexpanded node.  There it expands it, runs a simulation
on it and back propigates*/
int MonteCarlo::search(NodePtr node, short player){
	/* To avoid unnessisary work we start by checking if this is a goal/loss node for the player */
	if (node->has_goal) return WIN;
	else if (node->has_loss) return LOSS;

	/* We get the legal moves for this board */
	vector<Board> moves = getLegalBoards(node->board, player);
	int result;
	NodePtr temp;
	
	/* If the node has no children we expand the first one if it has no winning move */
	if (node->children.empty()){
		if (moves.size() == 0) return LOSS;
		Board move = moves.front();
		for (size_t i = 0; i < moves.size(); i++){
			/* If it has a winning move we find it and turn it into a win or a loss node */
			if (Checkers::goalTest(moves[i], player) == WIN){
				move = moves[i];
				if (player == Checkers::getPlayer()) node->has_goal = true;
				else node->has_loss = true;
				break;
			}
		}
		if (!(node->has_goal || node->has_loss)) result = simulation(move, (player == 1) ? 2 : 1);
		else result = INT_MAX; //Default value for winning node to make sure its always picked
		/* A new node is created and updated */
		temp = new Node(0, 0, moves.front());
		updateNode(temp, player, result);
		node->children.push_back(temp); //WE add the node to the child vector
		s++;
	} else {
		/* If the node has children we have to find out if we should select one of them or expand a new node */
		double maxValue = -1; //Default max value
		NodePtr maxNode = NULL;
		double currValue;
		size_t i;
		/* We find the best child */
		for (i = 0; i < node->children.size(); i++){
			if ((currValue = evaluationUCB1(node->children[i])) > maxValue){
				maxValue = currValue;
				maxNode = node->children[i];
			}
		}
		/* We check if the best child is better than expanding a new node */
		if ((moves.size() > node->children.size()) && evaluationUCB1(NULL) > maxValue){
			/* New node is expanded rather than selecting an old one */
			result = simulation(moves[i], (player == 1) ? 2 : 1);
			temp = new Node(0, 0, moves[i]);
			updateNode(temp, player, result);
			node->children.push_back(temp);
			s++;
		}
		else{
			/* An old node is selected */
			temp = maxNode;
			result = search(temp, (player == 1) ? 2 : 1);
			//assert(result < 2);
			updateNode(temp, player, result);
		}
	}
	return result;

}

/* The updateNode function takes in a node and two shorts and updates the node with the correct sim_count and win_count. */
void MonteCarlo::updateNode(NodePtr node, short player, short result){
	node->sim_count++; //We always want to increment the sim_count
	switch (result){
	case WIN:
		/* If there is a win for the player we increment his win counter*/
		if (player == Checkers::getPlayer()){
			node->win_count++;
		}
		break;
	case LOSS:
		/* If there is a win for the opponent we increment his win counter*/
		if ((player != Checkers::getPlayer())){
			node->win_count++;
		}
		break;
	default:
		break;
	}

}

/*The simulation function takes in a board and randomly simulates it to the end of the game or until a draw is reached */

int MonteCarlo::simulation(Board board, short player){
	short me = Checkers::getPlayer(); //We get which is the player
	Board currMove = board; 
	int isGoal = 0;
	int count = last_count; //We initialize the count variable for the draw check
	int new_count = count;
	int last_cap = moves_since_last_capture;
	for (size_t i = 0; i < SIMULATION_LENGTH; i++){
		/* We start off by checking if there is a goal and every ten turns we check for a draw */
		isGoal = Checkers::goalTest(currMove, player);
		if (isGoal == WIN || isGoal == LOSS) break;
		if (i % 10 == 9) {			
			if ((new_count = Checkers::count(currMove)) == count){
				last_cap += 10;
			}
			if (last_cap >= 50) {
				isGoal = DRAW;
				break;
			}
		}
		/* We get the legal moves and return a draw if there are none. */
		vector<Board> moves = getLegalBoards(currMove, player);
		if (moves.size() == 0){
			isGoal = LOSS;
			break;
		}
		/* We pick a number uniformly at random */
		distr = uniform_int_distribution<int>(0, moves.size() - 1);
		currMove = moves[distr(generator)];
		player = (player == 1) ? 2 : 1; //And then we swap players
	}

	//To be more sophisticated later.
	if (isGoal == WIN) return (me == player ? WIN : LOSS);
	if (isGoal == LOSS) return(me == player ? LOSS : WIN);
	else return isGoal;
}

void MonteCarlo::initDB() {
	if (!db_initialised) {
		db_init(128, str);
		db_initialised = true;
	}
}