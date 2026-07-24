#include "board.h"
#include <algorithm>

Cell::Cell() = default;

Cell::Cell(char c, bool filled) : value(c - '0'), preFilled(filled) {}

Board::Board(const std::string& puzzle, std::array<std::array<int, 9>, 9> sol) : solution(sol)
{
    for (int i = 0; i < 81; i++)
    {
        if (puzzle[i] != '0')
        {
            grid[i / 9][i % 9] = Cell(puzzle[i], true);
        }
        else
        {
            grid[i / 9][i % 9] = Cell(puzzle[i], false);
        }
    }
}

void Board::MoveCaret(int dRow, int dCol)
{
    caret[0] = std::clamp(caret[0] + dRow, 0, 8);
    caret[1] = std::clamp(caret[1] + dCol, 0, 8);
}

int Board::GetValue()
{
    Cell& cell = grid[caret[0]][caret[1]];
    return cell.value;
}

void Board::SetValue(int val)
{
    Cell& cell = grid[caret[0]][caret[1]];
    if (!cell.preFilled)
    {
        cell.value = val;
    }
}

void Board::ClearCell()
{
    Cell& cell = grid[caret[0]][caret[1]];
    if (!cell.preFilled)
    {
        cell.value = 0;
    }
}

void Board::ClearCandidates()
{
    Cell& cell = grid[caret[0]][caret[1]];
    cell.candidates.reset();
}

void Board::ToggleCandidate(int val)
{
    Cell& cell = grid[caret[0]][caret[1]];
    cell.candidates.flip(val);
}