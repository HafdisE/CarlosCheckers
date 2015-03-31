#include "MaxHeap.h"


MaxHeap::MaxHeap()
{
	//Bananas in pajamas
}


MaxHeap::~MaxHeap()
{
}
NodePtr MaxHeap::top(){
	return heap.front();
}
void MaxHeap::push(NodePtr node){
	heap.push_back(node);
	push_heap(heap.begin(), heap.end(), compare::operator());
}

void MaxHeap::pop(){
	pop_heap(heap.begin(), heap.end(), compare::operator());
	heap.pop_back();
}

void MaxHeap::update_top(double value){
	heap[0]->worth = value;
	make_heap(heap.begin(), heap.end());
}

void MaxHeap::update_at(double value, int i){
	//Wtf this function makes no sense atm, what am i doing with my life?
}
