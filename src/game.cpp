#include "button.h"
#include "lib.h"

#include "controls.h"
#include "raylib.h"
//#include "rayui_impl.h"   // Not sure if i will be using it...
#include "tetris.h"
#include "tetris_block.h"
#include "tetromino.h"
#include "ui.h"


#include <iostream>
#include <stdint.h>

using namespace tetromino;

int main(void)
{
    // Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "yoyoTetris");
    SetTargetFPS(60);
    Rectangle* tetrisStage = new Rectangle({ 250, 40, 300, 450 });
    Rectangle* uiRectangle;
    
    auto gameUI = new tetrisUI();
    auto game = gameTetris(gameUI, tetromino::tetrominoNames::LightBlue_I);

    // Step
    while (!WindowShouldClose()) {
        BeginDrawing();

        // UI Update
        gameUI->Display();

        // Game Update
        game.Loop();
        if (game.gameFinished()) {
            gameUI->ChangeStage(gameStage::GAME_OVER);
        }

        // End
        EndDrawing();
    }

    // Stop
    delete gameUI;
    delete tetrisStage;
    CloseWindow();
    return 0;
}
