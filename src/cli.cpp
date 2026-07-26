#include "cli.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

namespace fs = std::filesystem;

std::string GetHomeDir()
{
    const char* home = std::getenv("HOME");
    if (!home)
    {
        home = std::getenv("USERPROFILE");
    }
    return home ? std::string(home) : ".";
}

std::string GetSudokuDir()
{
    return GetHomeDir() + "/.sudodoku";
}

std::string GetPuzzleFilePath(const std::string& difficulty)
{
    return GetSudokuDir() + "/puzzles_" + difficulty + ".txt";
}

std::string GetSolvedFilePath()
{
    return GetSudokuDir() + "/solved.txt";
}

void InitializePuzzleDirectory()
{
    std::string sudokuDir = GetSudokuDir();
    fs::create_directories(sudokuDir);

    // Create empty files if they don't exist
    for (const auto& difficulty : {"easy", "medium", "hard"})
    {
        std::string filepath = GetPuzzleFilePath(difficulty);
        if (!fs::exists(filepath))
        {
            std::ofstream file(filepath);
            file.close();
        }
    }

    std::string solvedPath = GetSolvedFilePath();
    if (!fs::exists(solvedPath))
    {
        std::ofstream file(solvedPath);
        file.close();
    }
}

std::vector<PuzzleEntry> ListPuzzles(const std::string& difficulty)
{
    std::vector<PuzzleEntry> puzzles;
    std::string filepath = GetPuzzleFilePath(difficulty);

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        return puzzles;
    }

    std::string line;
    int index = 0;
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            puzzles.push_back({line, index});
            index++;
        }
    }
    file.close();

    return puzzles;
}

std::string GetPuzzle(const std::string& difficulty, int index)
{
    auto puzzles = ListPuzzles(difficulty);
    if (index >= 0 && index < (int)puzzles.size())
    {
        return puzzles[index].puzzle;
    }
    return "";
}

void AddPuzzle(const std::string& puzzle, const std::string& difficulty)
{
    std::string filepath = GetPuzzleFilePath(difficulty);

    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open puzzle file: " + filepath);
    }

    file << puzzle << "\n";
    file.close();

    std::cout << "Puzzle added to " << difficulty << " category.\n";
}

void SaveSolvedPuzzle(const std::string& puzzle, const std::string& difficulty, float timeSeconds,
                      int mistakes)
{
    int minutes = (int)timeSeconds / 60;
    int seconds = (int)timeSeconds % 60;
    char timeStr[16];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);

    // Get current date
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char dateStr[16];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeinfo);

    // Append to solved.txt
    std::string solvedPath = GetSolvedFilePath();
    std::ofstream solvedFile(solvedPath, std::ios::app);
    if (!solvedFile.is_open())
    {
        throw std::runtime_error("Failed to open solved file: " + solvedPath);
    }

    solvedFile << puzzle << "|" << difficulty << "|" << timeStr << "|" << mistakes << "|"
               << dateStr << "\n";
    solvedFile.close();

    // Remove from unsolved puzzles
    std::string puzzleFilePath = GetPuzzleFilePath(difficulty);
    std::ifstream inFile(puzzleFilePath);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Failed to open puzzle file: " + puzzleFilePath);
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line))
    {
        if (line != puzzle)
        {
            lines.push_back(line);
        }
    }
    inFile.close();

    std::ofstream outFile(puzzleFilePath);
    for (const auto& l : lines)
    {
        if (!l.empty())
        {
            outFile << l << "\n";
        }
    }
    outFile.close();
}

std::vector<SolvedEntry> GetSolveHistory()
{
    std::vector<SolvedEntry> history;
    std::string solvedPath = GetSolvedFilePath();

    std::ifstream file(solvedPath);
    if (!file.is_open())
    {
        return history;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        // Parse: puzzle|difficulty|time|mistakes|date
        std::stringstream ss(line);
        std::string puzzle, difficulty, timeStr, mistakesStr, date;

        if (std::getline(ss, puzzle, '|') && std::getline(ss, difficulty, '|') &&
            std::getline(ss, timeStr, '|') && std::getline(ss, mistakesStr, '|') &&
            std::getline(ss, date, '|'))
        {
            history.push_back({puzzle, difficulty, 0, std::stoi(mistakesStr), date});
        }
    }
    file.close();

    return history;
}

void DisplayStatus()
{
    InitializePuzzleDirectory();

    int easyUnsolved = ListPuzzles("easy").size();
    int mediumUnsolved = ListPuzzles("medium").size();
    int hardUnsolved = ListPuzzles("hard").size();

    auto history = GetSolveHistory();
    int easySolved = 0, mediumSolved = 0, hardSolved = 0;
    for (const auto& entry : history)
    {
        if (entry.difficulty == "easy")
            easySolved++;
        else if (entry.difficulty == "medium")
            mediumSolved++;
        else if (entry.difficulty == "hard")
            hardSolved++;
    }

    std::cout << "\n=== Sudoku Status ===\n";
    std::cout << "Easy:   " << easyUnsolved << " unsolved, " << easySolved << " solved\n";
    std::cout << "Medium: " << mediumUnsolved << " unsolved, " << mediumSolved << " solved\n";
    std::cout << "Hard:   " << hardUnsolved << " unsolved, " << hardSolved << " solved\n";
    std::cout << "Total:  " << (easyUnsolved + mediumUnsolved + hardUnsolved) << " unsolved, "
              << (easySolved + mediumSolved + hardSolved) << " solved\n\n";
}
