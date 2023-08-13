#include "controls.h"
#include "tetris_block.h"
#include "ui.h"
#include <iostream>

int main(void)
{
    // Constants
    const int screenWidth = 760;
    const int screenHeight = 540;

    // Init
    InitWindow(screenWidth, screenHeight, "yoyoTetris");
    SetTargetFPS(60);
    Rectangle* gameRectangle = new Rectangle({ 250, 40, 300, 450 });
    Rectangle* uiRectangle;
    controlsTetris gameControls;
    staticTetrisBlocks staticBlocks;

    auto actualBlock = new floatTetrisBlock(gameRectangle, &gameControls);
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
            actualBlock = new floatTetrisBlock(gameRectangle, &gameControls);
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
