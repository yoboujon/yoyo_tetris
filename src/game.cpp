#include "lib.h"

#include "game/tetris.h"
#include "game/tetromino.h"

using namespace tetromino;

#ifdef _WIN32
#ifdef NDEBUG
#define main int WinMain(void)
#else
#define main int main(void)
#endif
#else
#define main int main(void)
#endif

main
{
    // Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "yoyoTetris");
    // Loading the icon
    Image icon = LoadImage("res/yoyotetris_logo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // Target FPS = 60 (may be configurable), no exit key
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    float elapsedTime = 0.0f;

    // TODO : Multi-threading for event, ui elements and score.
    auto gameEvent = tetrisEvent();
    auto gameUI = tetrisUI(&gameEvent, &elapsedTime);
    auto gameScore = tetrisScore(&gameEvent);
    auto game = tetrisGame(&gameEvent, &gameUI, &gameScore);

    // Step
    while (!WindowShouldClose() && !(gameUI.quitGame()))
    {
        elapsedTime += GetFrameTime();
        const auto actualStage = gameUI.getStage();
        const auto back = gameUI.getRenderTexture(renderLayer::BACK);
        const auto front = gameUI.getRenderTexture(renderLayer::FRONT);
        // std::cout << "<mouse Position> x: " << GetMousePosition().x << "\ty: " << GetMousePosition().y << std::endl;

        BeginTextureMode(*back); // Drawing the back texture
        gameUI.Display(renderLayer::BACK);

        // Game Display and Update
        if (actualStage != gameStage::TITLE_SCREEN)
        {
            game.Loop();
        }
        // Checking if the game need to be reset
        if (gameUI.checkGameState(gameState::RESET))
        {
            gameScore.resetScore();
            game.reset(&gameEvent, &gameUI, &gameScore);
            // Loading the textures only if we stay on stages that needs these textures.
            // As a matter of fact, when starting a game from the titlescreen : this function will be called again.
            if(actualStage != gameStage::TITLE_SCREEN)
                game.setTetrominoTexture(gameUI.getTetrominoTexture());
        }
        EndTextureMode();

        BeginTextureMode(*front); // UI Update Front
        gameUI.Display(renderLayer::FRONT);
        EndTextureMode();

        gameUI.DisplayTexture();
    }

    // Stop
    CloseWindow();
    return 0;
}