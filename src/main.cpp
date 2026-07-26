#include "board.h"
#include "cli.h"
#include "input.h"
#include "raylib.h"
#include "renderer.h"
#include "solver.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

void PrintUsage(const char* programName)
{
    std::cout << "Usage:\n";
    std::cout << "  " << programName << " list [easy|medium|hard]\n";
    std::cout << "  " << programName << " play <difficulty> [index]\n";
    std::cout << "  " << programName << " add <puzzle> <easy|medium|hard>\n";
    std::cout << "  " << programName << " history\n";
    std::cout << "  " << programName << " status\n";
    std::cout << "  " << programName << " ongoing\n";
    std::cout << "  " << programName << " (no args) -> opens GUI Menu\n";
}

enum class AppState { MENU, DIFFICULTY_SELECT, PUZZLE_SELECT, PLAYING, CONFIRM_RESET, HISTORY, ONGOING, EXIT };

int main(int argc, char* argv[])
{
    InitializePuzzleDirectory();

    if (argc > 1)
    {
        std::string command = argv[1];

        if (command == "status")
        {
            DisplayStatus();
            return 0;
        }
        else if (command == "history")
        {
            auto history = GetSolveHistory();
            if (history.empty())
            {
                std::cout << "No solved puzzles yet.\n";
                return 0;
            }
            std::cout << "\n=== Solve History ===\n";
            for (const auto& entry : history)
            {
                std::cout << entry.difficulty << " | " << entry.time << " | " << entry.mistakes
                          << " mistakes | " << entry.date << "\n";
            }
            std::cout << "\n";
            return 0;
        }
        else if (command == "ongoing")
        {
            auto ongoing = GetInProgressPuzzles();
            if (ongoing.empty())
            {
                std::cout << "No ongoing puzzles.\n";
                return 0;
            }
            std::cout << "\n=== Ongoing Puzzles ===\n";
            for (const auto& entry : ongoing)
            {
                int minutes = (int)entry.time / 60;
                int seconds = (int)entry.time % 60;
                printf("%s | Time: %02d:%02d | Mistakes: %d\n", entry.difficulty.c_str(), minutes, seconds, entry.mistakes);
                std::cout << "Puzzle: " << entry.originalPuzzle << "\n\n";
            }
            return 0;
        }
        else if (command == "list")
        {
            std::string difficulty = (argc > 2) ? argv[2] : "easy";
            auto puzzles = ListPuzzles(difficulty);
            if (puzzles.empty())
            {
                std::cout << "No unsolved puzzles in " << difficulty << " category.\n";
                return 0;
            }
            std::cout << "\n=== " << difficulty << " Puzzles ===\n";
            for (const auto& p : puzzles)
            {
                std::cout << "[" << p.index << "] " << p.puzzle << "\n";
            }
            std::cout << "\n";
            return 0;
        }
        else if (command == "add")
        {
            if (argc < 4)
            {
                std::cerr << "Usage: " << argv[0] << " add <puzzle> <easy|medium|hard>\n";
                return 1;
            }
            AddPuzzle(argv[2], argv[3]);
            return 0;
        }
        else if (command != "play" && command != "menu")
        {
            PrintUsage(argv[0]);
            return 1;
        }
    }

    // GUI Mode
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1000, 1000, "Sudodoku");
    SetExitKey(0); // Disable ESC from exiting immediately

    AppState state = AppState::MENU;
    
    // Variables for playing state
    std::unique_ptr<Board> gameBoard;
    std::string currentDifficulty = "";
    std::string currentPuzzle = "";
    std::vector<PuzzleEntry> currentPuzzles;
    float elapsedTime = 0.0f;
    int currentPage = 0;
    bool isPaused = false;
    bool puzzleSolved = false;

    // GUI Mode Initialization checks if "play" was passed
    if (argc > 1)
    {
        std::string command = argv[1];
        if (command == "play")
        {
            if (argc < 3)
            {
                std::cerr << "Usage: " << argv[0] << " play <difficulty> [index]\n";
                CloseWindow();
                return 1;
            }
            currentDifficulty = argv[2];
            int index = 0;
            if (argc > 3)
            {
                try {
                    index = std::stoi(argv[3]);
                } catch (...) {
                    std::cerr << "Invalid index.\n";
                    CloseWindow();
                    return 1;
                }
            }
            currentPuzzle = GetPuzzle(currentDifficulty, index);
            if (currentPuzzle.empty())
            {
                std::cerr << "No unsolved puzzle found in " << currentDifficulty << " category at index " << index << ".\n";
                CloseWindow();
                return 1;
            }
            if (!ValidatePuzzleString(currentPuzzle))
            {
                std::cerr << "Puzzle must contain exactly 81 digits.\n";
                CloseWindow();
                return 1;
            }
            PuzzleSolution solution = SolvePuzzle(currentPuzzle);
            if (!solution.solvable)
            {
                std::cerr << "Puzzle has no unique solution.\n";
                CloseWindow();
                return 1;
            }

            gameBoard = std::make_unique<Board>(currentPuzzle, solution.solution);
            elapsedTime = 0.0f;
            LoadInProgressPuzzle(currentPuzzle, *gameBoard, elapsedTime);
            state = AppState::PLAYING;
        }
    }

    auto history = GetSolveHistory();
    auto ongoing = GetInProgressPuzzles();

    while (!WindowShouldClose() && state != AppState::EXIT)
    {
        BeginDrawing();
        ClearBackground(BACKGROUND);

        switch (state)
        {
            case AppState::MENU:
            {
                int action = DrawMainMenu();
                if (action == 0) state = AppState::DIFFICULTY_SELECT;
                else if (action == 1) 
                {
                    ongoing = GetInProgressPuzzles();
                    currentPage = 0;
                    state = AppState::ONGOING;
                }
                else if (action == 2) 
                {
                    history = GetSolveHistory();
                    currentPage = 0;
                    state = AppState::HISTORY;
                }
                else if (action == 3) state = AppState::EXIT;
                break;
            }
            case AppState::DIFFICULTY_SELECT:
            {
                int action = DrawDifficultyMenu();
                if (action == 3) state = AppState::MENU; // Back
                else if (action >= 0 && action <= 2)
                {
                    currentDifficulty = (action == 0) ? "easy" : ((action == 1) ? "medium" : "hard");
                    currentPuzzles = ListPuzzles(currentDifficulty);
                    ongoing = GetInProgressPuzzles();
                    currentPage = 0;
                    state = AppState::PUZZLE_SELECT;
                }
                break;
            }
            case AppState::PUZZLE_SELECT:
            {
                int action = DrawPuzzleSelectScreen(currentPuzzles, ongoing, currentPage);
                if (action == (int)currentPuzzles.size()) state = AppState::DIFFICULTY_SELECT; // Back
                else if (action >= 0 && action < (int)currentPuzzles.size())
                {
                    currentPuzzle = currentPuzzles[action].puzzle;
                    if (!currentPuzzle.empty() && ValidatePuzzleString(currentPuzzle))
                    {
                        PuzzleSolution solution = SolvePuzzle(currentPuzzle);
                        if (solution.solvable)
                        {
                            gameBoard = std::make_unique<Board>(currentPuzzle, solution.solution);
                            elapsedTime = 0.0f;
                            LoadInProgressPuzzle(currentPuzzle, *gameBoard, elapsedTime);
                            state = AppState::PLAYING;
                            puzzleSolved = false;
                            isPaused = false;
                        }
                    }
                }
                break;
            }
            case AppState::HISTORY:
            {
                if (DrawHistoryScreen(history, currentPage)) state = AppState::MENU;
                break;
            }
            case AppState::ONGOING:
            {
                int action = DrawOngoingScreen(ongoing, currentPage);
                if (action == (int)ongoing.size()) state = AppState::MENU; // Back
                else if (action >= 0 && action < (int)ongoing.size())
                {
                    currentDifficulty = ongoing[action].difficulty;
                    currentPuzzle = ongoing[action].originalPuzzle;
                    PuzzleSolution solution = SolvePuzzle(currentPuzzle);
                    if (solution.solvable)
                    {
                        gameBoard = std::make_unique<Board>(currentPuzzle, solution.solution);
                        elapsedTime = 0.0f;
                        LoadInProgressPuzzle(currentPuzzle, *gameBoard, elapsedTime);
                        state = AppState::PLAYING;
                        puzzleSolved = false;
                        isPaused = false;
                    }
                }
                break;
            }
            case AppState::PLAYING:
            {
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    // Save and exit to menu
                    if (!puzzleSolved)
                    {
                        SaveInProgressPuzzle(currentPuzzle, currentDifficulty, *gameBoard, elapsedTime);
                    }
                    else
                    {
                        RemoveInProgressPuzzle(currentPuzzle);
                    }
                    state = AppState::MENU;
                    break;
                }
                if (IsKeyPressed(KEY_P) && !puzzleSolved)
                {
                    isPaused = !isPaused;
                }
                if (IsKeyPressed(KEY_R) && !puzzleSolved)
                {
                    state = AppState::CONFIRM_RESET;
                }

                if (!gameBoard->IsSolved())
                {
                    if (!isPaused)
                    {
                        elapsedTime += GetFrameTime();
                        HandleInput(*gameBoard);
                    }
                }
                else if (!puzzleSolved)
                {
                    puzzleSolved = true;
                    RemoveInProgressPuzzle(currentPuzzle);
                    SaveSolvedPuzzle(currentPuzzle, currentDifficulty, elapsedTime, gameBoard->mistakes);
                }

                DrawBoard(*gameBoard, elapsedTime);

                if (gameBoard->IsSolved())
                {
                    DrawVictoryScreen(elapsedTime, gameBoard->mistakes);
                }
                else if (isPaused)
                {
                    DrawPauseScreen();
                }
                
                break;
            }
            case AppState::CONFIRM_RESET:
            {
                if (IsKeyPressed(KEY_Y))
                {
                    PuzzleSolution solution = SolvePuzzle(currentPuzzle);
                    if (solution.solvable)
                    {
                        gameBoard = std::make_unique<Board>(currentPuzzle, solution.solution);
                        elapsedTime = 0.0f;
                        RemoveInProgressPuzzle(currentPuzzle);
                        isPaused = false;
                    }
                    state = AppState::PLAYING;
                }
                else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE))
                {
                    state = AppState::PLAYING;
                }

                DrawBoard(*gameBoard, elapsedTime);
                DrawConfirmResetScreen();
                break;
            }
            case AppState::EXIT:
                break;
        }

        EndDrawing();
    }

    // Final cleanup if window closed via X while playing
    if (state == AppState::PLAYING)
    {
        if (!puzzleSolved && gameBoard)
        {
            SaveInProgressPuzzle(currentPuzzle, currentDifficulty, *gameBoard, elapsedTime);
        }
    }

    CloseWindow();
    return 0;
}
