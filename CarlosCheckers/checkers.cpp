#include "checkers.h"

Board::Board() {

}

Board::~Board() {

}

Checkers::Checkers(int player) {
	this->player = player;
	current_board = newBoard();
}

Checkers::~Checkers() {

}

Board Checkers::getBoard() {
	return current_board;
}

int Checkers::getPlayer() {
	return player;
}

void Checkers::updateBoard(Board board) {
	current_board = board;
}

void Checkers::updateBoard(struct CBmove *move) {
	current_board = applyMove(current_board, move);
}

Board applyMove(Board board, struct CBmove *move) {
	return board;
}

Board newBoard() {
	return Board();
}

struct CBmove** getLegalMoves(Board board) {
	return NULL;
}