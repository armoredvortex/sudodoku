#include "raylib.h"
#include "raymath.h"

// Draws a dashed line with custom thickness
void DrawLineDashedEx(Vector2 startPos, Vector2 endPos, float thickness, int dashSize, int spaceSize, Color color) 
{
	Vector2 delta = Vector2Subtract(endPos, startPos);
    float length = Vector2Length(delta);
 
    if (length == 0.0f) return;

    Vector2 direction = Vector2Scale(delta, 1.0f / length);
    float currentLength = 0.0f;

    while (currentLength < length) 
    {
        // Calculate the end of the current dash segment
        float nextLength = currentLength + dashSize;
        if (nextLength > length) nextLength = length;

        Vector2 dashStart = Vector2Add(startPos, Vector2Scale(direction, currentLength));
        Vector2 dashEnd = Vector2Add(startPos, Vector2Scale(direction, nextLength));

        // Use DrawLineEx to apply thickness to the segment
        DrawLineEx(dashStart, dashEnd, thickness, color);

        // Advance past the dash and the empty space
        currentLength += dashSize + spaceSize;
    }
}