#include "board.h"
#include "input.h"
#include "raylib.h"
#include "renderer.h"
#include "solver.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <81-character puzzle string>\n";
        return 1;
    }

    std::string puzzle = argv[1];

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

    // game loop
    while (!WindowShouldClose()) // run the loop until the user presses ESCAPE or presses the Close
                                 // button on the window
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
    return 0;
}
