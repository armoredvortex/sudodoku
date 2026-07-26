#include "renderer.h"
#include "board.h"
#include "raylib.h"
#include <cstdio>

void DrawBoard(const Board& board, float elapsedTime)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    constexpr int margin = 100;

    int boardSize = std::min(width, height) - 2 * margin;
    int cellSize = boardSize / 9;
    int fontSize = cellSize / 2;

    // Make sure the board is exactly divisible by 9
    boardSize = cellSize * 9;

    int totalUIHeight = boardSize;

    int boardX = (width - boardSize) / 2;

    int boardY = (height - totalUIHeight) / 2;
    Cell HighlightedCell = board.grid[board.caret[0]][board.caret[1]];

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            int x = boardX + col * cellSize;
            int y = boardY + row * cellSize;

            Cell cell = board.grid[row][col];
            char val[] = {(char)(cell.value + '0'), '\0'};

            if (board.caret[0] == row && board.caret[1] == col)
            {
                DrawRectangle(x, y, cellSize, cellSize, HIGHLIGHTED);
            }
            else if (HighlightedCell.value && HighlightedCell.value == cell.value)
            {
                DrawRectangle(x, y, cellSize, cellSize, SAME_VALUE);
            }
            else if (cell.preFilled)
            {
                DrawRectangle(x, y, cellSize, cellSize, PREFILLED);
            }

            if (cell.value && cell.value != board.solution[row][col])
            {
                DrawRectangle(x, y, cellSize, cellSize, WRONG);
            }
            else if (!cell.value)
            {
                int miniCell = cellSize / 3;
                int noteFont = cellSize / 4;

                for (int n = 1; n <= 9; n++)
                {
                    if (!cell.candidates.test(n))
                        continue;

                    int miniRow = (n - 1) / 3;
                    int miniCol = (n - 1) % 3;

                    int noteX = x + miniCol * miniCell;
                    int noteY = y + miniRow * miniCell;

                    char text[] = {char('0' + n), '\0'};

                    int w = MeasureText(text, noteFont);

                    DrawText(text, noteX + (miniCell - w) / 2, noteY + (miniCell - noteFont) / 2,
                             noteFont, NOTE);
                }
            }

            if (val[0] != '0')
            {
                int textWidth = MeasureText(val, fontSize);

                DrawText(val, x + (cellSize - textWidth) / 2, y + (cellSize - fontSize) / 2,
                         fontSize, NUM);
            }
        }
    }

    for (int i = 0; i <= 9; i++)
    {
        float thickness = (i % 3 == 0) ? 4.0f : 2.0f;
        Color LineColor = (i % 3 == 0) ? LINE_PRIMARY : LINE_SECONDARY;

        DrawLineEx({(float)(boardX + i * cellSize), (float)boardY},
                   {(float)(boardX + i * cellSize), (float)(boardY + boardSize)}, thickness,
                   LineColor);

        DrawLineEx({(float)boardX, (float)(boardY + i * cellSize)},
                   {(float)(boardX + boardSize), (float)(boardY + i * cellSize)}, thickness,
                   LineColor);
    }

    // Draw timer
    int minutes = (int)elapsedTime / 60;
    int seconds = (int)elapsedTime % 60;
    char timeText[16];
    snprintf(timeText, sizeof(timeText), "%02d:%02d", minutes, seconds);

    int timerFontSize = 40;
    int timerWidth = MeasureText(timeText, timerFontSize);
    int timerX = width - timerWidth - 50;
    int timerY = 50;

    DrawText(timeText, timerX, timerY, timerFontSize, NUM);

    // Draw mistakes counter
    char mistakesText[16];
    snprintf(mistakesText, sizeof(mistakesText), "Mistakes: %d", board.mistakes);

    int mistakesX = 50;
    int mistakesY = 50;
    DrawText(mistakesText, mistakesX, mistakesY, timerFontSize, WRONG);
}

void DrawVictoryScreen(float elapsedTime, int mistakes)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    // Semi-transparent overlay
    DrawRectangle(0, 0, width, height, {0, 0, 0, 200});

    // Victory message
    const char* victoryText = "Puzzle Solved!";
    int victoryFontSize = 80;
    int victoryWidth = MeasureText(victoryText, victoryFontSize);
    int victoryX = (width - victoryWidth) / 2;
    int victoryY = (height / 2) - 150;
    DrawText(victoryText, victoryX, victoryY, victoryFontSize, NUM);

    // Display stats
    int minutes = (int)elapsedTime / 60;
    int seconds = (int)elapsedTime % 60;
    char statsText[64];
    snprintf(statsText, sizeof(statsText), "Time: %02d:%02d | Mistakes: %d", minutes, seconds,
             mistakes);

    int statsFontSize = 40;
    int statsWidth = MeasureText(statsText, statsFontSize);
    int statsX = (width - statsWidth) / 2;
    int statsY = victoryY + victoryFontSize + 50;
    DrawText(statsText, statsX, statsY, statsFontSize, NUM);
}

void DrawPauseScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    // Semi-transparent overlay
    DrawRectangle(0, 0, width, height, {0, 0, 0, 200});

    // Pause message
    const char* pauseText = "PAUSED";
    int pauseFontSize = 80;
    int pauseWidth = MeasureText(pauseText, pauseFontSize);
    int pauseX = (width - pauseWidth) / 2;
    int pauseY = (height / 2) - 150;
    DrawText(pauseText, pauseX, pauseY, pauseFontSize, NUM);

    // Instructions
    const char* subText = "Press P to Unpause";
    int subFontSize = 40;
    int subWidth = MeasureText(subText, subFontSize);
    int subX = (width - subWidth) / 2;
    int subY = pauseY + pauseFontSize + 50;
    DrawText(subText, subX, subY, subFontSize, NUM);
}