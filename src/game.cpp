#include "lib.h"

#include "game/tetris.h"
#include "game/tetromino.h"
#include "graphics/render.h"
#include "event/gameevent.h"

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
    auto gameUI = tetrisUI();
    auto gameScore = tetrisScore();
    auto game = tetrisGame(&elapsedTime);
    auto gameEventHandler = GameEvent(&gameUI, &gameScore, &game, &elapsedTime);

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
        // TODO : Modify the escape key check in the tetrisGame to be here with the gameControls object
        // TODO : This could lead to a huge if statement when adding scenes in the future
        if (actualStage != gameStage::TITLE_SCREEN)
        {
            game.Loop();
            gameScore.updateScore();
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