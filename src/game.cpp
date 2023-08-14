#include "controls.h"
#include "tetris_block.h"
#include "tetromino.h"
#include "ui.h"
#include <iostream>
#include <stdint.h>

using namespace tetromino;

int main(void)
{
    // Constants
    const int screenWidth = 760;
    const int screenHeight = 540;
    std::map<tetrominoNames, tetrominoBlock> tetrominoMap;

    // Init
    InitWindow(screenWidth, screenHeight, "yoyoTetris");
    SetTargetFPS(60);
    Rectangle* gameRectangle = new Rectangle({ 250, 40, 300, 450 });
    Rectangle* uiRectangle;
    controlsTetris gameControls;
    staticTetrisBlocks staticBlocks;
    uint8_t name = 0;

    auto actualBlock = new floatTetrisBlock(static_cast<tetrominoNames>(name), gameRectangle, &gameControls);
    // Step
    while (!WindowShouldClose()) {
        BeginDrawing();
        // Init base element
        uiRectangle = initUI(gameRectangle);

        // Game Update

        // Falling block + static display
        staticBlocks.Display();
        actualBlock->Fall(staticBlocks.getRectangles());
        actualBlock->Rotate();
        actualBlock->Move(staticBlocks.getRectangles());

        // Checking for collision
        if (actualBlock->Placed()) {
            // TODO : Random Block created
            (name == 6) ? name = 0 : name++;
            staticBlocks.Add(*actualBlock, actualBlock->getColor());
            delete actualBlock;
            actualBlock = new floatTetrisBlock(static_cast<tetrominoNames>(name), gameRectangle, &gameControls);
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
