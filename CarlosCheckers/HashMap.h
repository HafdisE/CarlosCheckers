#pragma once
#include <list>
#include <vector>
#include "board.h"
#include <iostream>

using namespace std;
class HashMap
{
public:
	HashMap();
	~HashMap();
	int size(){ return N; }
	bool isEmpty(){ return !N; }
	bool contains(const Board& board);
	bool get(Board board, vector<Board>& moves);
	void put(vector<Board>, Board);
private:
	int INIT_CAPACITY = 10000;
	size_t N, M;
	vector< list<pair<vector<Board>, Board> >> st;
	void resize();

	
};




