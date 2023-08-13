#include "controls.h"
#include "tetris_block.h"
#include "tetromino.h"
#include "ui.h"
#include <iostream>

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

    auto actualBlock = new floatTetrisBlock(tetrominoNames::LightBlue_I, gameRectangle, &gameControls);
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
            actualBlock = new floatTetrisBlock(tetrominoNames::Yellow_O, gameRectangle, &gameControls);
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
