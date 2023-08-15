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
    Rectangle* gameRectangle = new Rectangle({ 250, 40, 300, 450 });
    Rectangle* uiRectangle;
    auto game = gameTetris(gameRectangle, tetromino::tetrominoNames::LightBlue_I);
    Vector2 mousePoint = { 0.0f, 0.0f };

    // Step
    while (!WindowShouldClose()) {
        // Mouse position update
        mousePoint = GetMousePosition();

        BeginDrawing();
        // Init base element
        uiRectangle = initUI(gameRectangle);

        // Game Update
        game.Loop();
        if (game.gameFinished()) {
            game.GameOverScreen();
        }

        // End
        EndDrawing();
    }

    // Stop
    delete uiRectangle;
    delete gameRectangle;
    CloseWindow();
    return 0;
}
