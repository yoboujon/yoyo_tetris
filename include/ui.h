#ifndef HEADER_TETRIS_UI
#define HEADER_TETRIS_UI

#include "lib.h"
#include "button.h"
#include "raylib.h"

constexpr Rectangle TILESET = { 0,0,16.0f,16.0f };

enum class gameStage {
    TITLE_SCREEN,
    GAME,
    GAME_OVER
};

enum class renderLayer {
    BACK,
    FRONT
};

class tetrisUI {
public:
    tetrisUI(float* elapsedPtr);
    ~tetrisUI();
    void Display(renderLayer layer);

    //Setters
    void ChangeStage(gameStage stage);

    //Getters
    Rectangle* getTetrisStage();
    float* getElapsedTime();
    bool quitGame();
    bool newGame();
private:
    void TileSet();
    void TitleScreen();
    void Game();
    void GameOver();

    // Stage
    gameStage _stage;
    float* _elapsedPtr;

    // Compositions
    Rectangle _Rect_tetrisStage;
    tetrisButton _Btn_restart;
    tetrisButton _Btn_quit;

    // Textures
    Texture2D _Texture_button;
    Texture2D _Texture_tileset_w;
    Texture2D _Texture_settings_w;

    // UI States
    bool _exit;
    bool _newGame;
};

#endif // HEADER_TETRIS_UI