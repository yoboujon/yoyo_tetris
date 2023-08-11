#include "ui.h"
#include "raylib.h"

Rectangle* initUI(Rectangle* gameRectangle)
{
    Rectangle* UIrectangle = new Rectangle({250, 40, 300, 460});
    ClearBackground(RAYWHITE);
    DrawText("YoyoTetris", 40, 40, 30, DARKGRAY);
    DrawText("Score", 40, 120, 20, DARKGRAY);
    DrawText("Level", 40, 300, 20, DARKGRAY);
    DrawText("Next", 650, 40, 20, DARKGRAY);
    DrawRectangleRec(*UIrectangle, DARKGRAY);
    DrawRectangleGradientEx(*gameRectangle, WHITE, LIGHTGRAY, LIGHTGRAY, WHITE);
    return UIrectangle;
}