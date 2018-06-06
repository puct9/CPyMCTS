#define BOOST_PYTHON_STATIC_LIB
#include "boost/python/tuple.hpp"

#include "search.h"
#include "node.h"
#include <iostream>
#include <vector>
#include <algorithm>


boost::python::tuple search(Board inBoard, int team, unsigned long iterations)
{
	std::cout << "Iterations being run: " << iterations << std::endl;
	if (!inBoard.getMoves())
	{
		std::cout << "Defaulting to middle of board for first move." << std::endl;
		return boost::python::make_tuple(inBoard.getSize() / 2, inBoard.getSize() / 2);
	}

	Node topNode;
	topNode.team_multiplier = -team;

	int info_every(std::max(unsigned long(2000), iterations / 5));

	/////////////////////////////// M C T S //////////////////////////////////
	for (unsigned long iteration = 0; iteration < iterations; ++iteration)
	{
		if (0 == iteration % info_every)
		{
			std::cout << "Progress %: " << iteration * 100 / iterations << std::endl;
			viewStats(&topNode, team);
		}
		Board tmp_board{ inBoard };
		Node* bestLeaf = topNode.SelectBest(&tmp_board);
		if (bestLeaf->terminal)
		{
			bestLeaf->BackPropagateScore(bestLeaf->termscore);
			continue;
		}
		if (!bestLeaf->visits)
		{
			int score = simulation(tmp_board, false);
			bestLeaf->BackPropagateScore(score);
			continue;
		}
		// Expand
		std::vector<std::array<int, 2>> obvMoves = tmp_board.getObvMoves();
		if (obvMoves.size() == 0)
		{
			bestLeaf->terminal = true;
			bestLeaf->termscore = 0; // tie
		}
		for (const std::array<int, 2>&move : obvMoves)
		{
			tmp_board.playMove(move[0], move[1]);
			int termscore = tmp_board.checkWin();
			if (termscore) bestLeaf->CreateChild(topNode.visits, move[0], move[1], termscore);
			bestLeaf->CreateChild(topNode.visits, move[0], move[1]);
			// Undo the move
			tmp_board.undoMove();
		}
	}

	// Suggest move

	int bestmove[2];
	unsigned long maxvisits = 0;
	for (const std::unique_ptr<Node>& move : topNode.children)
	{
		if (move->visits > maxvisits)
		{
			maxvisits = move->visits;
			bestmove[0] = move->move_to[0];
			bestmove[1] = move->move_to[1];
		}
	}
	std::cout << "Suggest: " << bestmove[0] << "," << bestmove[1] << std::endl;
	std::cout << "Win %:   " << team * topNode.score * 100 / topNode.visits << std::endl;

	return boost::python::make_tuple(bestmove[0], bestmove[1]);
}


void viewStats(Node * n, int t)
{
	if (n->children.size() == 0) return;
	int bestmove[2];
	unsigned long maxvisits = 0;
	for (const std::unique_ptr<Node>& move : n->children)
	{
		if (move->visits > maxvisits)
		{
			maxvisits = move->visits;
			bestmove[0] = move->move_to[0];
			bestmove[1] = move->move_to[1];
		}
	}
	if (maxvisits == 0 || n->visits == 0) return;
	std::cout << "Think: " << bestmove[0] << "," << bestmove[1] << std::endl;
	std::cout << "Win %: " << t * n->score * 100 / n->visits << "\n" << std::endl;
}


int simulation(Board board)
{
	return simulation(board, true);
}

int simulation(Board board, bool rebuild)
{
	if (rebuild)
	{
		/* Rebuild the board states to make sure it works properly
		   Will not guarantee proper working order initially, but
		       for the purpose of this function, it will suffice
		*/
		std::vector<std::array<int, 2>> tmp;
		for (int r = 0; r < board.getSize(); ++r)
		{
			for (int c = 0; c < board.getSize(); ++c)
			{
				if (board.getBoardPos(r, c)) tmp.push_back(std::array<int, 2>{ r, c });
			}
		}
		board.manOverridePlayedPieces(tmp);
	}

	while (board.getMoves() < pow(board.getSize(), 2))
	{
		// Make a random move
		std::vector<std::array<int, 2>> choices = board.getObvMoves();
		std::array<int, 2> randMove = choices[rand() % choices.size()];
		board.playMove(randMove[0], randMove[1]);
		if (board.checkWin()) return board.checkWin();
	}
	return 0;
}
