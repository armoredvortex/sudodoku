#include "board.h"
#include "cli.h"
#include "input.h"
#include "raylib.h"
#include "renderer.h"
#include "solver.h"

#include <cstdio>
#include <iostream>
#include <string>

void PrintUsage(const char* programName)
{
    std::cout << "Usage:\n";
    std::cout << "  " << programName << " list [easy|medium|hard]\n";
    std::cout << "  " << programName << " play <difficulty>\n";
    std::cout << "  " << programName << " add <puzzle> <easy|medium|hard>\n";
    std::cout << "  " << programName << " history\n";
    std::cout << "  " << programName << " status\n";
}

int main(int argc, char* argv[])
{
    InitializePuzzleDirectory();

    if (argc < 2)
    {
        PrintUsage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    // Handle CLI commands
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
    else if (command == "play")
    {
        if (argc < 3)
        {
            std::cerr << "Usage: " << argv[0] << " play <difficulty>\n";
            return 1;
        }
        std::string difficulty = argv[2];
        std::string puzzle = GetPuzzle(difficulty, 0);

        if (puzzle.empty())
        {
            std::cerr << "No unsolved puzzles in " << difficulty << " category.\n";
            return 1;
        }

        if (!ValidatePuzzleString(puzzle))
        {
            std::cerr << "Puzzle must contain exactly 81 digits.\n";
            return 1;
        }

        PuzzleSolution solution = SolvePuzzle(puzzle);
        if (!solution.solvable)
        {
            std::cerr << "Puzzle has no unique solution.\n";
            return 1;
        }

        Board gameBoard(puzzle, solution.solution);

        // Tell the window to use vsync and work on high DPI displays
        SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

        // Create the window and OpenGL context
        InitWindow(1000, 1000, "Sudodoku");

        float elapsedTime = 0.0f;
        bool puzzleSolved = false;

        // game loop
        while (!WindowShouldClose())
        {
            // drawing
            BeginDrawing();

            // Setup the back buffer for drawing (clear color and depth buffers)
            ClearBackground(BACKGROUND);

            if (!gameBoard.IsSolved())
            {
                elapsedTime += GetFrameTime();
                HandleInput(gameBoard);
            }
            else
            {
                puzzleSolved = true;
            }

            DrawBoard(gameBoard, elapsedTime);

            if (gameBoard.IsSolved())
            {
                DrawVictoryScreen(elapsedTime, gameBoard.mistakes);
            }

            // end the frame and get ready for the next one  (display frame, poll input, etc...)
            EndDrawing();
        }

        // destroy the window and cleanup the OpenGL context
        CloseWindow();

        // Save solved puzzle if it was completed
        if (puzzleSolved)
        {
            SaveSolvedPuzzle(puzzle, difficulty, elapsedTime, gameBoard.mistakes);
            std::cout << "Puzzle saved! Time: ";
            int minutes = (int)elapsedTime / 60;
            int seconds = (int)elapsedTime % 60;
            printf("%02d:%02d", minutes, seconds);
            std::cout << " | Mistakes: " << gameBoard.mistakes << "\n";
        }

        return 0;
    }

    PrintUsage(argv[0]);
    return 1;
}
