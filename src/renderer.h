#include "board.h"
#include "raylib.h"

void DrawBoard(const Board& board);

void DrawLineDashedEx(Vector2 startPos, Vector2 endPos, float thickness, int dashSize,
                      int spaceSize, Color color);