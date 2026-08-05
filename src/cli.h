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

struct InProgressEntry
{
    std::string originalPuzzle;
    std::string difficulty;
    float time;
    int mistakes;
};

// Get the path to the bundled font, checking installed locations then local resources/
std::string GetFontPath();

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

// Helper to get in-progress file path
std::string GetInProgressFilePath();

// Save in-progress puzzle
void SaveInProgressPuzzle(const std::string& originalPuzzle, const std::string& difficulty,
                          class Board& board, float time);

// Load in-progress puzzle. Returns true if found and loaded.
bool LoadInProgressPuzzle(const std::string& originalPuzzle, class Board& board, float& time);

// Remove in-progress puzzle
void RemoveInProgressPuzzle(const std::string& originalPuzzle);

// View in-progress games
std::vector<InProgressEntry> GetInProgressPuzzles();
