#include "raylib.h"
#include "tetris_block.h"
#include "ui.h"


int main(void)
{
    // Constants
    const int screenWidth = 760;
    const int screenHeight = 540;

    // Init
    InitWindow(screenWidth, screenHeight, "yoyoTetris");
    SetTargetFPS(60);
    Rectangle* gameRectangle = new Rectangle({ 254, 44, 292, 452 });
    Rectangle* uiRectangle;
    auto actualBlock = new floatTetrisBlock(gameRectangle);
    staticTetrisBlocks staticBlocks;
    // Step
    while (!WindowShouldClose()) {
        BeginDrawing();
        // Init base element
        uiRectangle = initUI(gameRectangle);

        // Game Update
        staticBlocks.Display();
        actualBlock->Fall(staticBlocks.getRectangles());
        actualBlock->Move(staticBlocks.getRectangles());
        if (actualBlock->Placed()) {
            staticBlocks.Add(*actualBlock);
            delete actualBlock;
            actualBlock = new floatTetrisBlock(gameRectangle);
        }

        // End
        EndDrawing();
    }

    // Stop
    delete uiRectangle;
    delete gameRectangle;
    CloseWindow();
    return 0;
}
