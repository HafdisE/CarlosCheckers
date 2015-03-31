#include "HashMap.h"

#include <iostream>

HashMap::HashMap() {
	N = 0;
	M = INIT_CAPACITY;
	st = vector< list<pair<vector<Board>, Board> >>(INIT_CAPACITY);
}


HashMap::~HashMap() {
}

bool HashMap::contains(const Board& board) {
	size_t i = board.hash() % M;

	for each (pair< vector<Board>, Board> item in st[i]){
		if (board == item.second) return true;
	}
	return false;
}

bool HashMap::get(Board board, vector<Board>& moves){
	size_t i = board.hash() % M;
	for each (pair< vector<Board>, Board> item in st[i]){
		if (board == item.second){
			moves = item.first;
			return true;
		}
	}
	return false;
}

void HashMap::put(vector<Board> moves, Board board){
	size_t i = board.hash() % M;
	for each (pair< vector<Board>, Board> item in st[i]){
		if (board == item.second) return;
	}
	N++;
	if (N > (5 * M)) resize();
	st[i].push_back(make_pair(moves, board));
}

void HashMap::resize(){
	int new_cap = M * 2;
	vector<list<pair<vector<Board>, Board>> > temp(new_cap);
	for (size_t i = 0; i < M; i++) {
		for each (pair< vector<Board>, Board> item in st[i]){
			int index = item.second.hash() % new_cap;
			temp[index].push_back(item);
		}
	}
	M *= 2;
	st = temp;
}
