#include "button.h"
#include "lib.h"

#include "controls.h"
#include "raylib.h"
// #include "rayui_impl.h"   // Not sure if i will be using it...
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
        const auto actualStage = gameUI->getStage();
        const auto back = *(gameUI->getRenderTexture(renderLayer::BACK));
        const auto front = *(gameUI->getRenderTexture(renderLayer::FRONT));
        // std::cout << "<mouse Position> x: " << GetMousePosition().x << "\ty: " << GetMousePosition().y << std::endl;

        BeginTextureMode(back); // Drawing the back texture
        gameUI->Display(renderLayer::BACK);
        // Game Display and Update
        if (actualStage == gameStage::GAME || actualStage == gameStage::GAME_OVER || actualStage == gameStage::MENU_SCREEN) {
            game->Loop();
            if(game->pause()) {
                gameUI->ChangeStage(gameStage::MENU_SCREEN);
            }
            if (game->gameFinished()) {
                gameUI->ChangeStage(gameStage::GAME_OVER);
            }
            if (gameUI->newGame()) {
                delete game;
                game = new gameTetris(gameUI, tetromino::tetrominoNames::LightBlue_I);
                gameUI->ChangeStage(gameStage::GAME);
            }
        }
        EndTextureMode();

        BeginTextureMode(front); // UI Update Front
        gameUI->Display(renderLayer::FRONT);
        EndTextureMode();

        BeginDrawing();
        gameUI->DisplayShader(renderLayer::BACK);
        DrawTextureRec(back.texture, { 0, 0, (float)back.texture.width, (float)-back.texture.height }, { 0, 0 }, WHITE);
        gameUI->DisplayShader(renderLayer::BACK, true);
        gameUI->DisplayShader(renderLayer::FRONT);
        DrawTextureRec(front.texture, { 0, 0, (float)front.texture.width, (float)-front.texture.height }, { 0, 0 }, WHITE);
        gameUI->DisplayShader(renderLayer::FRONT, true);
        EndDrawing();
    }

    // Stop
    delete game;
    delete gameUI;
    delete tetrisStage;
    CloseWindow();
    return 0;
}