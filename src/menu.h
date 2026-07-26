#pragma once
#include "board.h"

enum class GameState
{
    Menu,
    Playing,
    Paused
};

bool DrawMainMenu(GameState& state, Board& board);
void DrawPausedMenu(GameState& state);