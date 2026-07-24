#pragma once

#include <array>
#include <bitset>
#include <string>

class Cell
{
  public:
    int value;
    std::bitset<10> candidates;
    bool preFilled = false;
    Cell();
    Cell(char c, bool filled);
};

class Board
{
  public:
    bool CandidateMode = false;
    std::array<std::array<Cell, 9>, 9> grid;
    std::array<std::array<int, 9>, 9> solution;
    std::array<int, 2> caret = {0, 0};
    Board(const std::string& puzzle, std::array<std::array<int, 9>, 9> sol);
    void MoveCaret(int dRow, int dCol);
    int GetValue();
    void SetValue(int val);
    void ToggleCandidate(int val);
    void ClearCell();
    void ClearCandidates();
};