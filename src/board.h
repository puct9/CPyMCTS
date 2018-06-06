#pragma once
#include <vector>
#include <array>


// Class declaration and class method declarations go in the .h file here

class Board
{
private:
	short board[23][23];
	int lastPlaced[2];
	int size;
	int moves;
	std::vector<std::array<int, 2>> playedPieces;

public:

	Board(); // Default initialization to 19x19 board
	Board(int); // Initialization to NxN board

	std::string showString();
	void print();
	void reset();
	void playMove(int, int);
	void undoMove();
	void replayMoves();
	void manOverridePlayedPieces(std::vector<std::array<int, 2>> ov);
	int checkWin();
	int getMoves();
	int getSize();
	int getBoardPos(int r, int c);

	std::vector<std::array<int, 2>> getObvMoves();
};
