#include "ui.h"
#include "tetris_block.h"

int main(void)
{
    const int screenWidth = 760;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "yoyoTetris");
    SetTargetFPS(60);
    auto actualBlock = new tetrisBlock;
    while (!WindowShouldClose()) {
        BeginDrawing();
        initUI();
        actualBlock->Move();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}