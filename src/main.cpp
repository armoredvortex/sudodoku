#include <iostream>
#include "raylib.h"
#include "renderer.h"
#include "solver.h"

int main (int argc, char* argv[])
{
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " <puzzle>\n";
		return 1;
	}

	std::string puzzle = argv[1];
	if(!ValidatePuzzleString(puzzle)){
		std::cerr << "Invalid Puzzle format\n";
		return 1;
	}

	PuzzleSolution sol = SolvePuzzle(puzzle);
	if(!sol.solvable){
		std::cerr << "Puzzle doesn't have a valid/unique solution!\n";
		return 1;
	}

	std::cout << "Hello World!\n";

	// // Tell the window to use vsync and work on high DPI displays
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// // Create the window and OpenGL context
	// InitWindow(1000, 1000, "Sudodoku");

	// // game loop
	// while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	// {
	// 	// drawing
	// 	BeginDrawing();

	// 	// Setup the back buffer for drawing (clear color and depth buffers)
	// 	ClearBackground((Color){26,26,26,255});

	// 	// draw some text using the default font
	// 	// DrawText("Hello Raylib", 200,200,20,WHITE);
	// 	DrawLineDashedEx((Vector2){100, 100}, (Vector2){100, 900},5, 30, 30,(Color){210, 210, 210, 255});
		
	// 	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	// 	EndDrawing();
	// }

	// // destroy the window and cleanup the OpenGL context
	// CloseWindow();
	return 0;
}
