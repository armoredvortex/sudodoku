#pragma once

#include "board.h"
#include "raylib.h"

const Color BACKGROUND = {24, 22, 22, 255};
const Color LINE_SECONDARY = {168, 161, 190, 155};
const Color LINE_PRIMARY = {214, 214, 214, 155};
const Color HIGHLIGHTED = {147, 140, 108, 100};
const Color PREFILLED = {13, 12, 12, 255};
const Color NUM = {215, 215, 215, 255};
const Color WRONG = {200, 0, 0, 100};
const Color NOTE = {255, 255, 255, 200};

void DrawBoard(const Board& board);

void DrawLineDashedEx(Vector2 startPos, Vector2 endPos, float thickness, int dashSize,
                      int spaceSize, Color color);