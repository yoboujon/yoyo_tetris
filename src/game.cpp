#include "ui.h"

int main(void)
{
    const int screenWidth = 760;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "yoyoTetris");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        initUI();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}