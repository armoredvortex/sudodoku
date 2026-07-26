#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "board.h"
#include "menu.h"
#include "raylib.h"
#include "renderer.h"
#include "solver.h"

bool DrawMainMenu(GameState& state, Board& board)
{
    static char puzzleText[82] =
        "530070000600195000098000060800060003400803001700020006060000280000419005000080079";

    static bool editMode = false;
    static std::string error;

    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int centerX = width / 2;

    // Calculate scale, but clamp it so the UI doesn't become microscopic or absurdly huge
    float scale = std::min((float)width / 1000.0f, (float)height / 1200.0f);
    scale = std::max(0.5f, std::min(scale, 2.5f));

    // Scaled sizes
    int titleSize = (int)(72 * scale); // Slightly larger for emphasis
    int subtitleSize = (int)(28 * scale);
    int guiTextSize = (int)(24 * scale); // Size for raygui elements
    int errorTextSize = (int)(22 * scale);

    // Scaled Dimensions
    float panelWidth = 760 * scale; // A bit wider to fit 81 chars comfortably
    float elementHeight = 65 * scale;
    float spacing = 35 * scale;

    // 1. Calculate the total height of the UI block
    float totalUIHeight = titleSize + subtitleSize + (elementHeight * 2) + (spacing * 3);

    // 2. Dynamically calculate starting Y to vertically center the UI (slightly above center looks
    // best)
    float currentY = (height - totalUIHeight) / 2.5f;

    // --- TITLE ---
    DrawText("Sudodoku", centerX - MeasureText("Sudodoku", titleSize) / 2, (int)currentY, titleSize,
             NUM);
    currentY += titleSize + (spacing * 0.5f);

    // --- SUBTITLE ---
    DrawText("Enter an 81-character Sudoku puzzle",
             centerX - MeasureText("Enter an 81-character Sudoku puzzle", subtitleSize) / 2,
             (int)currentY, subtitleSize, LINE_SECONDARY);
    currentY += subtitleSize + (spacing * 1.5f);

    // --- RAYGUI ELEMENTS ---
    // Save current gui text size, then apply our scaled text size
    int originalGuiTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, guiTextSize);

    // Text Box
    Rectangle textBox = {(float)centerX - panelWidth / 2, currentY, panelWidth, elementHeight};
    if (GuiTextBox(textBox, puzzleText, 82, editMode))
    {
        editMode = !editMode;
    }
    currentY += elementHeight + spacing;

    // Start Button
    Rectangle startButton = {textBox.x, currentY, panelWidth, elementHeight};
    if (GuiButton(startButton, "Start Game"))
    {
        std::string puzzle = puzzleText;

        if (!ValidatePuzzleString(puzzle))
        {
            error = "Puzzle must contain exactly 81 digits.";
        }
        else
        {
            PuzzleSolution sol = SolvePuzzle(puzzle);

            if (!sol.solvable)
            {
                error = "Puzzle has no unique solution.";
            }
            else
            {
                board = Board(puzzle, sol.solution);
                state = GameState::Playing;
                error.clear();

                // Always restore GUI style before returning!
                GuiSetStyle(DEFAULT, TEXT_SIZE, originalGuiTextSize);
                return true;
            }
        }
    }
    currentY += elementHeight + spacing;

    // Restore original GUI style after drawing gui elements
    GuiSetStyle(DEFAULT, TEXT_SIZE, originalGuiTextSize);

    // --- ERROR MESSAGE ---
    if (!error.empty())
    {
        DrawText(error.c_str(), centerX - MeasureText(error.c_str(), errorTextSize) / 2,
                 (int)currentY, errorTextSize, RED);
    }

    return false;
}

void DrawPausedMenu(GameState& state) {}