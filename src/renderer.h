#pragma once

#include "board.h"
#include "raylib.h"

const Color BACKGROUND = {30, 30, 36, 255};      // Deep gray with a hint of navy
const Color LINE_SECONDARY = {80, 80, 95, 255};  // Visible cool-gray
const Color LINE_PRIMARY = {190, 200, 220, 255}; // Bright icy silver
const Color HIGHLIGHTED = {38, 79, 120, 180};    // Classic editor blue
const Color PREFILLED = {36, 36, 44, 255};       // Elevated cool gray
const Color NUM = {250, 250, 255, 255};          // Icy white
const Color WRONG = {244, 71, 71, 220};          // Standard VS Code error red
const Color NOTE = {150, 150, 165, 255};         // Crisp light cool-gray
const Color SAME_VALUE = {38, 79, 120, 100};     // Faded classic editor blue

void DrawBoard(const Board& board, float elapsedTime = 0.0f);

void DrawVictoryScreen(float elapsedTime, int mistakes);

void DrawPauseScreen();

void DrawLineDashedEx(Vector2 startPos, Vector2 endPos, float thickness, int dashSize,
                      int spaceSize, Color color);