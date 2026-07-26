#pragma once

#include <string>
#include <vector>

struct PuzzleEntry
{
    std::string puzzle;
    int index;
};

struct SolvedEntry
{
    std::string puzzle;
    std::string difficulty;
    int time;
    int mistakes;
    std::string date;
};

// Initialize ~/.sudodoku directory
void InitializePuzzleDirectory();

// List unsolved puzzles
std::vector<PuzzleEntry> ListPuzzles(const std::string& difficulty);

// Get a specific puzzle
std::string GetPuzzle(const std::string& difficulty, int index);

// Add new puzzle
void AddPuzzle(const std::string& puzzle, const std::string& difficulty);

// Save solved puzzle and remove from unsolved
void SaveSolvedPuzzle(const std::string& puzzle, const std::string& difficulty, float timeSeconds,
                      int mistakes);

// View history of solved puzzles
std::vector<SolvedEntry> GetSolveHistory();

// Display status
void DisplayStatus();

// Helper to get puzzle file path
std::string GetPuzzleFilePath(const std::string& difficulty);

// Helper to get solved file path
std::string GetSolvedFilePath();
