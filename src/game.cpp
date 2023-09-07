#include "event/component.h"
#include "lib.h"

#include "game/tetris.h"
#include "game/tetromino.h"
#include "graphics/render.h"
#include "event/gameevent.h"
#include "raylib.h"
#include "raymath.h"

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
    Vector2 mousePosition = { 0 };

    // TODO : Multi-threading for event, ui elements and score.
    auto& renderer = TetrisRenderer::GetInstance();
    auto gameUI = tetrisUI();
    auto gameScore = tetrisScore();
    auto game = tetrisGame(&elapsedTime);
    auto gameEventHandler = GameEvent(&gameUI, &gameScore, &game, &elapsedTime);

    // Step
    while (!WindowShouldClose() && !(gameUI.quitGame()))
    {
        if( !Vector2Equals(mousePosition, GetMousePosition()) )
        {
            gameEventHandler.sendEvent(nullptr, EventType::MOUSE_MOVED);
            mousePosition = GetMousePosition();
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            gameEventHandler.sendEvent(nullptr, EventType::LEFT_CLICK);
        }
        elapsedTime += GetFrameTime();
        RendererLayer layer = RendererLayer::BACK;

        renderer.BeginDisplay(layer);
        gameUI.Display(layer);
        renderer.EndDisplay();

        layer = RendererLayer::GAME;
        renderer.BeginDisplay(layer);
        // Game Display and Update
        if (renderer.GetStage() == gameStage::GAME || renderer.GetStage() == gameStage::MENU_SCREEN)
        {
            game.Loop();
            // Only updating the score if the stage is the game.
            if(renderer.GetStage() == gameStage::GAME)
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