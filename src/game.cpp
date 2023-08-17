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
    SetExitKey(KEY_NULL);
    Rectangle* tetrisStage = new Rectangle({ 250, 40, 300, 450 });
    Rectangle* uiRectangle;
    float elapsedTime = 0.0f;
    
    auto gameUI = new tetrisUI(&elapsedTime);
    auto game = new gameTetris(gameUI, tetromino::tetrominoNames::LightBlue_I);

    // Step
    while (!WindowShouldClose() && !(gameUI->quitGame())) {
        elapsedTime += GetFrameTime();
        auto actualStage = gameUI->getStage();
        std::cout << "<mouse Position> x: " << GetMousePosition().x << "\ty: " << GetMousePosition().y << std::endl;
        BeginDrawing();

        // UI Update Back
        gameUI->Display(renderLayer::BACK);

        // Game Update
        if(actualStage == gameStage::GAME || actualStage == gameStage::GAME_OVER)
        {
            game->Loop();
            if (game->gameFinished()) {
                gameUI->ChangeStage(gameStage::GAME_OVER);
            }
            if(gameUI->newGame())
            {
                delete game;
                game = new gameTetris(gameUI, tetromino::tetrominoNames::LightBlue_I);
                gameUI->ChangeStage(gameStage::GAME);
            }
        }

        // UI Update Front (after the game rendering.)
        gameUI->Display(renderLayer::FRONT);

        // End
        EndDrawing();
    }

    // Stop
    delete game;
    delete gameUI;
    delete tetrisStage;
    CloseWindow();
    return 0;
}
