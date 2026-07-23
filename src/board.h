#pragma once

#include <array>
#include <bitset>
#include <string>

class Cell
{
    int value;
    std::bitset<10> candidates;

public:
    Cell();
    Cell(char c);
};

class Board
{
    std::array<std::array<Cell, 9>, 9> grid;

public:
    Board(const std::string& puzzle);
};