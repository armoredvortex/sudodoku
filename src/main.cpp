#include "board.h"
#include "input.h"
#include "menu.h"
#include "raygui.h"
#include "raylib.h"
#include "renderer.h"
#include "solver.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Board GameBoard;

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(1200, 1200, "Sudodoku");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 12);

    GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 2);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BACKGROUND));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(LINE_PRIMARY));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(NUM));

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BACKGROUND));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(HIGHLIGHTED));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(PREFILLED));

    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(BACKGROUND));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(LINE_PRIMARY));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(NUM));

    GameState state = GameState::Menu;

    // game loop
    while (!WindowShouldClose()) // run the loop until the user presses ESCAPE or presses the Close
                                 // button on the window
    {

        // drawing
        BeginDrawing();

        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(BACKGROUND);

        switch (state)
        {
        case GameState::Menu:
            DrawMainMenu(state, GameBoard);
            break;

        case GameState::Playing:
            HandleInput(GameBoard);
            DrawBoard(GameBoard);
            break;

        case GameState::Paused:
            DrawBoard(GameBoard);
            DrawPausedMenu(state);
        }

        // end the frame and get ready for the next one  (display frame, poll input, etc...)
        EndDrawing();
    }

    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
