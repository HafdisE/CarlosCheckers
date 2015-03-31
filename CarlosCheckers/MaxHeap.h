#ifndef MAXHEAP_H
#define MAXHEAP_H 
#pragma once
#include "node.h"
#include <vector>

class MaxHeap
{
public:
	MaxHeap();
	~MaxHeap();
	NodePtr top();
	void push(NodePtr node);
	void pop();
	void update_top(double value);
	void update_at(double value, int i);
private:
	vector<NodePtr> heap;
};

#endif