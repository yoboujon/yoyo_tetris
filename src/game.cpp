#include "event/component.h"
#include "lib.h"

#include "game/tetris.h"
 
#include "graphics/render.h"
#include "user/mouse.h"
#include "user/input_manager.h"

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
    auto& mouse = TetrisMouse::GetInstance();
    auto& input = TetrisInputManager::GetInstance();
    auto gameUI = tetrisUI();
    auto gameScore = tetrisScore();
    auto game = tetrisGame(&elapsedTime);
    auto gameEventHandler = GameEvent(&gameUI, &gameScore, &game, &elapsedTime);

    // Step
    while (!WindowShouldClose() && !(gameUI.quitGame()))
    {
        // Loading state for textures
        const bool loadingState = gameEventHandler.getLoading();
        // Elapsed time
        elapsedTime += GetFrameTime();
        // Mouse cursor style update
        mouse.UpdateMouse();
        input.Update();

        // Events for Mouse/Keyboard
        if( !Vector2Equals(mousePosition, GetMousePosition()) )
        {
            gameEventHandler.sendEvent(nullptr, EventType::MOUSE_MOVED);
            mousePosition = GetMousePosition();
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            gameEventHandler.sendEvent(nullptr, EventType::LEFT_CLICK);

        // Displaying only if not loading a scene (see getLoading on gameEvent)
        renderer.BeginDisplay(loadingState);
        renderer.RenderTexture(RendererLayer::BACK);
        gameUI.Display(RendererLayer::BACK);

        // Game Display and Update
        // RendererLayer::GAME
        if (renderer.GetStage() == gameStage::GAME || renderer.GetStage() == gameStage::MENU_SCREEN)
        {
            game.Loop();
            // Only updating the score if the stage is the game.
            if(renderer.GetStage() == gameStage::GAME)
                gameScore.updateScore();
        }
        renderer.RenderTexture(RendererLayer::GAME);

        renderer.RenderTexture(RendererLayer::FRONT);
        gameUI.Display(RendererLayer::FRONT);

        //renderer.Render();
        renderer.EndDisplay(loadingState);
    }

    // Stop
    CloseWindow();
    return 0;
}