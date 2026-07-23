#include "input.h"
#include "board.h"
#include "raylib.h"

void HandleMovement(Board& board)
{
    if (IsKeyPressed(KEY_RIGHT) | IsKeyPressed(KEY_D))
        board.MoveCaret(0, +1);

    if (IsKeyPressed(KEY_LEFT) | IsKeyPressed(KEY_A))
        board.MoveCaret(0, -1);

    if (IsKeyPressed(KEY_UP) | IsKeyPressed(KEY_W))
        board.MoveCaret(-1, 0);

    if (IsKeyPressed(KEY_DOWN) | IsKeyPressed(KEY_S))
        board.MoveCaret(+1, 0);
}

void HandleNumInput(Board& board)
{
    int ch = GetCharPressed();

    while (ch > 0)
    {
        if (ch >= '1' && ch <= '9')
            board.SetValue(ch - '0');

        if (ch == '0')
            board.ClearCell();

        ch = GetCharPressed();
    }
}

void HandleInput(Board& board)
{
    HandleMovement(board);
    HandleNumInput(board);
}
