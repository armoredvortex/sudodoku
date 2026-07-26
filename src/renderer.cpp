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

bool DrawButton(const char* text, int x, int y, int width, int height)
{
    Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
    Vector2 mousePoint = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePoint, rect);

    DrawRectangleRec(rect, isHovered ? HIGHLIGHTED : PREFILLED);
    DrawRectangleLinesEx(rect, 2, LINE_SECONDARY);

    int fontSize = 30;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, x + (width - textWidth) / 2, y + (height - fontSize) / 2, fontSize, NUM);

    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int DrawMainMenu()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    const char* title = "sudodoku";
    int titleSize = 80;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (width - titleW) / 2, 150, titleSize, NUM);

    int btnWidth = 300;
    int btnHeight = 60;
    int startY = 350;
    int spacing = 90;
    int btnX = (width - btnWidth) / 2;

    if (DrawButton("Play New Game", btnX, startY, btnWidth, btnHeight))
        return 0;
    if (DrawButton("Resume Game", btnX, startY + spacing, btnWidth, btnHeight))
        return 1;
    if (DrawButton("View History", btnX, startY + spacing * 2, btnWidth, btnHeight))
        return 2;
    if (DrawButton("Exit", btnX, startY + spacing * 3, btnWidth, btnHeight))
        return 3;

    return -1;
}

int DrawDifficultyMenu()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    const char* title = "SELECT DIFFICULTY";
    int titleSize = 60;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (width - titleW) / 2, 150, titleSize, NUM);

    int btnWidth = 300;
    int btnHeight = 60;
    int startY = 350;
    int spacing = 90;
    int btnX = (width - btnWidth) / 2;

    if (DrawButton("Easy", btnX, startY, btnWidth, btnHeight))
        return 0;
    if (DrawButton("Medium", btnX, startY + spacing, btnWidth, btnHeight))
        return 1;
    if (DrawButton("Hard", btnX, startY + spacing * 2, btnWidth, btnHeight))
        return 2;
    if (DrawButton("Back", btnX, startY + spacing * 3, btnWidth, btnHeight))
        return 3;

    return -1;
}

void DrawConfirmResetScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    // Semi-transparent overlay
    DrawRectangle(0, 0, width, height, Fade(BACKGROUND, 0.8f));

    const char* pauseText = "CONFIRM RESET";
    int pauseFontSize = 60;
    int pauseWidth = MeasureText(pauseText, pauseFontSize);
    int pauseX = (width - pauseWidth) / 2;
    int pauseY = height / 2 - 100;
    DrawText(pauseText, pauseX, pauseY, pauseFontSize, NOTE);

    const char* subText = "Are you sure you want to reset?";
    int subFontSize = 40;
    int subWidth = MeasureText(subText, subFontSize);
    int subX = (width - subWidth) / 2;
    int subY = pauseY + pauseFontSize + 50;
    DrawText(subText, subX, subY, subFontSize, NUM);

    const char* subText2 = "(Y to confirm, N to cancel)";
    int subFontSize2 = 30;
    int subWidth2 = MeasureText(subText2, subFontSize2);
    int subX2 = (width - subWidth2) / 2;
    int subY2 = subY + subFontSize + 20;
    DrawText(subText2, subX2, subY2, subFontSize2, NOTE);
}

bool DrawHistoryScreen(const std::vector<SolvedEntry>& history, int& currentPage)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    const char* title = "SOLVE HISTORY";
    int titleSize = 60;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (width - titleW) / 2, 80, titleSize, NUM);

    int y = 200;
    if (history.empty())
    {
        const char* msg = "No solved puzzles yet.";
        DrawText(msg, (width - MeasureText(msg, 30)) / 2, y, 30, NOTE);
    }
    else
    {
        int itemsPerPage = 8;
        int totalPages = (history.size() + itemsPerPage - 1) / itemsPerPage;
        if (currentPage >= totalPages) currentPage = totalPages - 1;
        if (currentPage < 0) currentPage = 0;

        int startIdx = currentPage * itemsPerPage;
        int endIdx = std::min(startIdx + itemsPerPage, (int)history.size());

        for (int i = startIdx; i < endIdx; i++)
        {
            const auto& entry = history[history.size() - 1 - i];
            int minutes = entry.time / 60;
            int seconds = entry.time % 60;
            char buf[128];
            snprintf(buf, sizeof(buf), "%s | %s | %02d:%02d | Mistakes: %d", entry.date.c_str(),
                     entry.difficulty.c_str(), minutes, seconds, entry.mistakes);
            DrawText(buf, (width - MeasureText(buf, 25)) / 2, y, 25, NUM);
            y += 40;
        }

        int btnWidth = 150;
        int btnHeight = 50;
        if (currentPage > 0)
        {
            if (DrawButton("Prev", 50, height - 100, btnWidth, btnHeight)) currentPage--;
        }
        if (currentPage < totalPages - 1)
        {
            if (DrawButton("Next", width - 50 - btnWidth, height - 100, btnWidth, btnHeight)) currentPage++;
        }
        char pageBuf[32];
        snprintf(pageBuf, sizeof(pageBuf), "Page %d / %d", currentPage + 1, totalPages);
        DrawText(pageBuf, (width - MeasureText(pageBuf, 20)) / 2, height - 140, 20, NUM);
    }

    int btnWidth = 200;
    int btnHeight = 50;
    return DrawButton("Back", (width - btnWidth) / 2, height - 100, btnWidth, btnHeight);
}

int DrawOngoingScreen(const std::vector<InProgressEntry>& ongoing, int& currentPage)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    const char* title = "ONGOING GAMES";
    int titleSize = 60;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (width - titleW) / 2, 80, titleSize, NUM);

    int btnWidth = 200;
    int btnHeight = 50;
    bool backClicked =
        DrawButton("Back", (width - btnWidth) / 2, height - 100, btnWidth, btnHeight);
    if (backClicked)
        return ongoing.size(); // special value for back

    int y = 200;
    if (ongoing.empty())
    {
        const char* msg = "No ongoing games.";
        DrawText(msg, (width - MeasureText(msg, 30)) / 2, y, 30, NOTE);
        return -1;
    }
    else
    {
        int itemsPerPage = 7;
        int totalPages = (ongoing.size() + itemsPerPage - 1) / itemsPerPage;
        if (currentPage >= totalPages) currentPage = totalPages - 1;
        if (currentPage < 0) currentPage = 0;

        int startIdx = currentPage * itemsPerPage;
        int endIdx = std::min(startIdx + itemsPerPage, (int)ongoing.size());

        int slotWidth = 600;
        int slotHeight = 60;
        int slotX = (width - slotWidth) / 2;

        for (int i = startIdx; i < endIdx; i++)
        {
            const auto& entry = ongoing[i];
            int minutes = entry.time / 60;
            int seconds = (int)entry.time % 60;
            char buf[128];
            snprintf(buf, sizeof(buf), "%s | %02d:%02d | Mistakes: %d", entry.difficulty.c_str(),
                     minutes, seconds, entry.mistakes);

            if (DrawButton(buf, slotX, y, slotWidth, slotHeight))
                return i;
            y += slotHeight + 20;
        }

        int pbtnWidth = 150;
        if (currentPage > 0)
        {
            if (DrawButton("Prev", 50, height - 100, pbtnWidth, btnHeight)) currentPage--;
        }
        if (currentPage < totalPages - 1)
        {
            if (DrawButton("Next", width - 50 - pbtnWidth, height - 100, pbtnWidth, btnHeight)) currentPage++;
        }
        char pageBuf[32];
        snprintf(pageBuf, sizeof(pageBuf), "Page %d / %d", currentPage + 1, totalPages);
        DrawText(pageBuf, (width - MeasureText(pageBuf, 20)) / 2, height - 140, 20, NUM);
    }
    return -1;
}

int DrawPuzzleSelectScreen(const std::vector<PuzzleEntry>& puzzles, const std::vector<InProgressEntry>& ongoing, int& currentPage)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    
    const char* title = "SELECT PUZZLE";
    int titleSize = 60;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (width - titleW) / 2, 80, titleSize, NUM);
    
    int btnWidth = 200;
    int btnHeight = 50;
    bool backClicked = DrawButton("Back", (width - btnWidth) / 2, height - 100, btnWidth, btnHeight);
    if (backClicked) return puzzles.size(); // special value for back

    int y = 200;
    if (puzzles.empty())
    {
        const char* msg = "No unsolved puzzles available.";
        DrawText(msg, (width - MeasureText(msg, 30)) / 2, y, 30, NOTE);
        return -1;
    }
    else
    {
        int itemsPerPage = 7;
        int totalPages = (puzzles.size() + itemsPerPage - 1) / itemsPerPage;
        if (currentPage >= totalPages) currentPage = totalPages - 1;
        if (currentPage < 0) currentPage = 0;

        int startIdx = currentPage * itemsPerPage;
        int endIdx = std::min(startIdx + itemsPerPage, (int)puzzles.size());

        int slotWidth = 700;
        int slotHeight = 60;
        int slotX = (width - slotWidth) / 2;
        
        for (int i = startIdx; i < endIdx; i++)
        {
            const auto& entry = puzzles[i];
            
            bool isActive = false;
            for (const auto& on : ongoing)
            {
                if (on.originalPuzzle == entry.puzzle)
                {
                    isActive = true;
                    break;
                }
            }

            char buf[128];
            if (isActive)
            {
                snprintf(buf, sizeof(buf), "[ACTIVE] Puzzle %d: %.*s...", entry.index, 20, entry.puzzle.c_str());
            }
            else
            {
                snprintf(buf, sizeof(buf), "Puzzle %d: %.*s...", entry.index, 20, entry.puzzle.c_str());
            }
            
            if (DrawButton(buf, slotX, y, slotWidth, slotHeight)) return i;
            y += slotHeight + 20;
        }

        int pbtnWidth = 150;
        if (currentPage > 0)
        {
            if (DrawButton("Prev", 50, height - 100, pbtnWidth, btnHeight)) currentPage--;
        }
        if (currentPage < totalPages - 1)
        {
            if (DrawButton("Next", width - 50 - pbtnWidth, height - 100, pbtnWidth, btnHeight)) currentPage++;
        }
        char pageBuf[32];
        snprintf(pageBuf, sizeof(pageBuf), "Page %d / %d", currentPage + 1, totalPages);
        DrawText(pageBuf, (width - MeasureText(pageBuf, 20)) / 2, height - 140, 20, NUM);
    }
    return -1;
}
