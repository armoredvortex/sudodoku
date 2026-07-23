#include "renderer.h"
#include "board.h"
#include "raylib.h"
// #include "raymath.h"

void DrawBoard(const Board& board)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    constexpr int margin = 100;

    int boardSize = std::min(width, height) - 2 * margin;
    int cellSize = boardSize / 9;
    int fontSize = cellSize / 2;

    // Make sure the board is exactly divisible by 9
    boardSize = cellSize * 9;

    int boardX = (width - boardSize) / 2;
    int boardY = (height - boardSize) / 2;

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
                             noteFont, WHITE);
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
}