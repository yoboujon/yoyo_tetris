#include "ui.h"
#include "raylib.h"

void initUI()
{
    ClearBackground(RAYWHITE);
    DrawText("YoyoTetris", 40, 40, 30, DARKGRAY);
    DrawText("Score", 40, 120, 20, DARKGRAY);
    DrawText("Level", 40, 300, 20, DARKGRAY);
    DrawText("Next", 650, 40, 20, DARKGRAY);
    DrawRectangle(250, 40, 300, 460, DARKGRAY);
    DrawRectangleGradientV(254, 44, 292, 452, WHITE, RAYWHITE);
}