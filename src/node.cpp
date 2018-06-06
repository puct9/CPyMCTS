#include "node.h"
#include "board.h"
#include <string>
#include <iostream>
#include <limits>


// Node implementations

Node::Node()
{
	// Default constuctor
	// Node * parentNode will be left undefined
	// It is the duty of the programmer to not do anything foolish
	parentNode = nullptr;
	visits = 0, score = 0, visit_offset = 0, team_multiplier = 1;
}

Node::Node(Node *parent, unsigned long offset, int mt_r, int mt_c, int term)
{
	// Constructor used when the node is not the top node.
	// int term default value 0
	parentNode = parent;
	move_to[0] = mt_r; move_to[1] = mt_c;
	visits = 0, score = 0, visit_offset = offset, team_multiplier = -parent->team_multiplier;
	if (term) terminal = true;
	termscore = term;
}

void Node::Backtrack()
{
	std::cout << "yeet" << std::endl;
	if (parentNode == nullptr) return;
	parentNode->Backtrack();
}

void Node::CreateChild(unsigned long offset, int mt_r, int mt_c, int term)
{
	// Creates a child and appends it to Node<std::unique_ptr<Node>> children
	// Nodes created here use the second constructor,
	// and `this` is the parent of its children, hence the `this` argument
	children.push_back(std::make_unique<Node>(this, offset, mt_r, mt_c, term));
}

void Node::BackPropagateScore(int s)
{
	++visits;
	score += s;
	if (parentNode == nullptr) return;
	parentNode->BackPropagateScore(s);
}

bool Node::IsLeaf()
{
	return !children.size();
}

double Node::ucb1()
{
	return ucb1(sqrt(2));
}

double Node::ucb1(double exploration)
{
	if (!visits) return std::numeric_limits<double>::infinity();
	double average = (double)score / visits;
	return team_multiplier * average + exploration * sqrt(log(parentNode->visits) / visits);
}

Node* Node::SelectBest(Board * board)
{
	if (parentNode != nullptr) board->playMove(move_to[0], move_to[1]);
	// This will only have effect on the top node.
	if (IsLeaf()) return this;
	double max_value = -1.0;
	int max_index = 0;
	for (unsigned int i = 0; i < children.size(); i++)
	{
		double tmp = children[i]->ucb1();
		if (tmp > max_value)
		{
			max_value = tmp;
			max_index = i;
		}
	}
	return children[max_index]->SelectBest(board);
}
