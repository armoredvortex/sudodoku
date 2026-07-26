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

bool NumberPressed(int value)
{
    static constexpr KeyboardKey topRow[] = {KEY_ONE, KEY_TWO,   KEY_THREE, KEY_FOUR, KEY_FIVE,
                                             KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE};

    static constexpr KeyboardKey keypad[] = {KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4, KEY_KP_5,
                                             KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9};

    return IsKeyPressed(topRow[value - 1]) || IsKeyPressed(keypad[value - 1]);
}

void HandleNumInput(Board& board)
{
    bool candidateMode = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    for (int i = 1; i <= 9; i++)
    {
        if (NumberPressed(i))
        {
            if (candidateMode)
                board.ToggleCandidate(i);
            else
            {
                if (board.GetValue() == i)
                    board.ClearCell();
                else
                    board.SetValue(i);
            }
        }
    }

    if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_KP_0))
    {
        if (candidateMode)
            board.ClearCandidates();
        else
            board.ClearCell();
    }
}

void HandleInput(Board& board)
{
    HandleMovement(board);
    HandleNumInput(board);
}
