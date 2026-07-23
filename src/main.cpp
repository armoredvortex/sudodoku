#include "board.h"
#include "input.h"
#include "raylib.h"
#include "renderer.h"
#include "solver.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <puzzle>\n";
        return 1;
    }

    std::string puzzle = argv[1];
    if (!ValidatePuzzleString(puzzle))
    {
        std::cerr << "Invalid Puzzle format\n";
        return 1;
    }

    PuzzleSolution sol = SolvePuzzle(puzzle);
    if (!sol.solvable)
    {
        std::cerr << "Puzzle doesn't have a valid/unique solution!\n";
        return 1;
    }

    std::cout << "Starting Game!\n";

    Board GameBoard = Board(puzzle, sol.solution);

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(1000, 1500, "Sudodoku");

    // game loop
    while (!WindowShouldClose()) // run the loop until the user presses ESCAPE or presses the Close
                                 // button on the window
    {
        HandleInput(GameBoard);

        // drawing
        BeginDrawing();

        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(BACKGROUND);

        // Draw Board
        DrawBoard(GameBoard);

        // end the frame and get ready for the next one  (display frame, poll input, etc...)
        EndDrawing();
    }

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
