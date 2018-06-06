#pragma once
#include "board.h"
#include <vector>
#include <string>
#include <memory>

// Node class

class Node
{
public:
	Node *parentNode;
	std::vector<std::unique_ptr<Node>> children;
	unsigned long visits;
	long long score;
	unsigned long visit_offset;
	int team_multiplier;
	int move_to[2];
	bool terminal;
	int termscore;

	// Constructors
	// Default. Will assume top node
	Node();
	// Norm
	Node(Node *parent, unsigned long offset, int mt_r, int mt_c, int term = 0);
	
	//Methods
	void Backtrack();
	void CreateChild(unsigned long offset, int mt_r, int mt_c, int term = 0);
	void BackPropagateScore(int s);
	bool IsLeaf();
	double ucb1();
	double ucb1(double exploration);
	Node* SelectBest(Board * board);
};
