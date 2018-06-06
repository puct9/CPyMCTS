#pragma once
#define BOOST_PYTHON_STATIC_LIB
#include "boost/python/tuple.hpp"

#include "board.h"
#include "node.h"


boost::python::tuple search(Board inBoard, int team, unsigned long iterations = 10000);
void viewStats(Node* n, int t);
int simulation(Board board);
int simulation(Board board, bool rebuild);
