#include "lib.h"

#include "rayui_impl.h"
#include "controls.h"
#include "tetromino.h"
#include "tetris_block.h"
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
        staticBlocks.checkLine();
        const auto& collisionStatic = staticBlocks.getRectangles();
        actualBlock->Fall(collisionStatic);
        actualBlock->Rotate(collisionStatic);
        actualBlock->Move(collisionStatic);

        // Checking for collision
        if (actualBlock->Placed()) {
            // TODO : Random Block created
            (name == 6) ? name = 0 : name++;
            staticBlocks.Add(*actualBlock, actualBlock->getColor());
            delete actualBlock;
            actualBlock = new floatTetrisBlock(static_cast<tetrominoNames>(name), gameRectangle, &gameControls);
            if (actualBlock->GameEnded(staticBlocks.getRectangles()))
            {
                std::cout << "Game Over." << std::endl;
                break;
            }
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
