#include <array>
#include <bitset>

class Cell {
	int value;
	std::bitset<10> candidates;
};

class Board {
	std::array<std::array<Cell, 9>, 9> grid;
};