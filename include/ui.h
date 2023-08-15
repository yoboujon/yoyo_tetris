#ifndef HEADER_TETRIS_UI
#define HEADER_TETRIS_UI

#include "lib.h"
#include "button.h"
#include "raylib.h"

enum class gameStage {
    TITLE_SCREEN,
    GAME,
    GAME_OVER
};

class tetrisUI {
public:
    tetrisUI();
    ~tetrisUI();
    void Display();

    //Setters
    void ChangeStage(gameStage stage);

    //Getters
    Rectangle* getTetrisStage();
private:
    void TitleScreen();
    void Game();
    void GameOver();

    // Stage
    gameStage _stage;

    // Compositions
    Rectangle _Rect_tetrisStage;
    tetrisButton _Btn_restart;
    tetrisButton _Btn_quit;

    // Textures
    Texture2D _Texture_button;
};

#endif // HEADER_TETRIS_UI