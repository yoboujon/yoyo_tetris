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
    auto& renderer = TetrisRenderer::GetInstance();
    auto gameUI = tetrisUI();
    auto gameScore = tetrisScore();
    auto game = tetrisGame(&elapsedTime);
    auto gameEventHandler = GameEvent(&gameUI, &gameScore, &game, &elapsedTime);

    // Step
    while (!WindowShouldClose() && !(gameUI.quitGame()))
    {
        elapsedTime += GetFrameTime();
        RendererLayer layer = RendererLayer::BACK;

        renderer.BeginDisplay(layer);
        gameUI.Display(layer);
        renderer.EndDisplay();

        layer = RendererLayer::GAME;
        renderer.BeginDisplay(layer);
        // Game Display and Update
        // TODO : Modify the escape key check in the tetrisGame to be here with the gameControls object
        // TODO : This could lead to a huge if statement when adding scenes in the future
        if (renderer.GetStage() != gameStage::TITLE_SCREEN)
        {
            game.Loop();
            gameScore.updateScore();
        }
        renderer.EndDisplay();

        layer = RendererLayer::FRONT;
        renderer.BeginDisplay(layer);
        gameUI.Display(layer);
        renderer.EndDisplay();

        renderer.Render();
    }

    // Stop
    CloseWindow();
    return 0;
}