#include "board.h"

Cell::Cell() = default;

Cell::Cell(char c)
    : value(c - '0')
{
}

Board::Board(const std::string& puzzle)
{
    for (int i = 0; i < 81; i++)
    {
        grid[i / 9][i % 9] = Cell(puzzle[i]);
    }
}