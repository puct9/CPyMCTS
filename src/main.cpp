#define BOOST_PYTHON_STATIC_LIB
#include <string>
#include "boost/python.hpp"
#include "boost/python/def.hpp"
#include "boost/python/module.hpp"

#include "board.h"
#include "search.h"


BOOST_PYTHON_FUNCTION_OVERLOADS(search_overloads, search, 2, 3);


BOOST_PYTHON_MODULE(cpymcts)
{
	using namespace boost::python;
	class_<Board>("Board", init<>())
		.def(init<int>())
		.def("reset", &Board::reset)
		.def("play", &Board::playMove)
		.def("undo", &Board::undoMove)
		.def("get", &Board::getBoardPos)
		.def("check_win", &Board::checkWin)
		.add_property("string", &Board::showString)
		.add_property("size", &Board::getSize)
		.add_property("move_count", &Board::getMoves)
		;
	def("search", search, search_overloads());
}
