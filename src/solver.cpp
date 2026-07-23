#include "solver.h"
#include <string>

bool ValidatePuzzleString(const std::string& puzzle)
{
    if (puzzle.size() != 81)
    {
        return false;
    }

    for (auto e : puzzle)
    {
        if (e < '0' || e > '9')
        {
            return false;
        }
    }

    return true;
}

bool IsValid(const std::array<std::array<int, 9>, 9>& grid, int row, int col, int num)
{
    // Check row
    for (int c = 0; c < 9; c++)
    {
        if (grid[row][c] == num)
            return false;
    }

    // Check column
    for (int r = 0; r < 9; r++)
    {
        if (grid[r][col] == num)
            return false;
    }

    // Check 3x3 box
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;

    for (int r = startRow; r < startRow + 3; r++)
    {
        for (int c = startCol; c < startCol + 3; c++)
        {
            if (grid[r][c] == num)
                return false;
        }
    }

    return true;
}

bool RecursiveSolve(int idx, std::array<std::array<int, 9>, 9>& grid, int& solutionCount,
                    std::array<std::array<int, 9>, 9>& solution)
{
    if (idx == 81)
    {
        solution = grid;
        solutionCount++;
        return solutionCount >= 2; // stop if we've found multiple solutions
    }

    int row = idx / 9;
    int col = idx % 9;

    if (grid[row][col] != 0)
    {
        return RecursiveSolve(idx + 1, grid, solutionCount, solution);
    }

    for (int num = 1; num <= 9; num++)
    {
        if (IsValid(grid, row, col, num))
        {
            grid[row][col] = num;

            if (RecursiveSolve(idx + 1, grid, solutionCount, solution))
                return true;

            grid[row][col] = 0;
        }
    }

    return false;
}

PuzzleSolution SolvePuzzle(const std::string& puzzle)
{
    std::array<std::array<int, 9>, 9> grid;
    for (int i = 0; i < puzzle.size(); i++)
    {
        grid[i / 9][i % 9] = puzzle[i] - '0';
    }

    int SolutionCount = 0;
    std::array<std::array<int, 9>, 9> solution;
    RecursiveSolve(0, grid, SolutionCount, solution);

    if (SolutionCount != 1)
    {
        return {false, {}};
    }

    return {true, solution};
}