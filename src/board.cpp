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
        if (val != 0 && val != solution[caret[0]][caret[1]])
        {
            mistakes++;
        }
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

bool Board::IsSolved() const
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col].value != solution[row][col])
            {
                return false;
            }
        }
    }
    return true;
}