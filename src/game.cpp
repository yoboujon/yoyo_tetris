#include "lib.h"

#include "game/tetris.h"
#include "game/tetromino.h"

using namespace tetromino;

#ifdef _WIN32
    #ifdef NDEBUG
    int WinMain(void)
    #else
    int main(void)
    #endif
#else
int main(void)
#endif
{
    // Init
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "yoyoTetris");
    //Loading the icon
    Image icon = LoadImage("res/yoyotetris_logo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // Target FPS = 60 (may be configurable), no exit key
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    float elapsedTime = 0.0f;

    auto gameEvent = new tetrisEvent();
    auto gameUI = new tetrisUI(gameEvent, &elapsedTime);
    auto gameScore = new tetrisScore(gameEvent);
    auto game = new tetrisGame(gameEvent, gameUI, gameScore);

    // Step
    while (!WindowShouldClose() && !(gameUI->quitGame())) {
        elapsedTime += GetFrameTime();
        const auto actualStage = gameUI->getStage();
        const auto back = gameUI->getRenderTexture(renderLayer::BACK);
        const auto front = gameUI->getRenderTexture(renderLayer::FRONT);
        //std::cout << "<mouse Position> x: " << GetMousePosition().x << "\ty: " << GetMousePosition().y << std::endl;

        BeginTextureMode(*back); // Drawing the back texture
        // Displaying the back of the environment
        gameUI->Display(renderLayer::BACK);
        // Game Display and Update
        if (actualStage != gameStage::TITLE_SCREEN) {
            game->Loop();
            // ! A problem has been noticed when returning to the title screen :
            // ! The game is not deleted and the textures aren't unloaded. This can lead
            // ! To potential memory leaks.
            if (gameUI->newGame()) {
                delete game;
                delete gameScore;
                gameScore = new tetrisScore(gameEvent);
                game = new tetrisGame(gameEvent, gameUI, gameScore);
                gameUI->ChangeStage(gameStage::GAME);
                game->setTetrominoTexture(gameUI->getTetrominoTexture());
            }
        }
        EndTextureMode();

        BeginTextureMode(*front); // UI Update Front
        gameUI->Display(renderLayer::FRONT);
        EndTextureMode();

        gameUI->DisplayTexture();
    }

    // Stop
    delete game;
    delete gameUI;
    CloseWindow();
    return 0;
}