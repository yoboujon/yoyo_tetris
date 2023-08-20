#include "lib.h"

#include "tetris.h"
#include "tetromino.h"

using namespace tetromino;

int main(void)
{
    // Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "yoyoTetris");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    float elapsedTime = 0.0f;

    auto gameEvent = new tetrisEvent();
    auto gameUI = new tetrisUI(gameEvent, &elapsedTime);
    auto gameScore = new tetrisScore();
    auto game = new tetrisGame(gameEvent, gameUI, gameScore, tetromino::tetrominoNames::LightBlue_I);

    // Step
    while (!WindowShouldClose() && !(gameUI->quitGame())) {
        elapsedTime += GetFrameTime();
        const auto actualStage = gameUI->getStage();
        const auto back = *(gameUI->getRenderTexture(renderLayer::BACK));
        const auto front = *(gameUI->getRenderTexture(renderLayer::FRONT));
        //std::cout << "<mouse Position> x: " << GetMousePosition().x << "\ty: " << GetMousePosition().y << std::endl;

        BeginTextureMode(back); // Drawing the back texture
        gameUI->Display(renderLayer::BACK);
        // Game Display and Update
        if (actualStage != gameStage::TITLE_SCREEN) {
            game->Loop();
            if (gameUI->newGame()) {
                delete game;
                delete gameScore;
                game = new tetrisGame(gameEvent, gameUI, gameScore, tetromino::tetrominoNames::LightBlue_I);
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
    CloseWindow();
    return 0;
}