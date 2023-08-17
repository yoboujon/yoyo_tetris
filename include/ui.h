#ifndef HEADER_TETRIS_UI
#define HEADER_TETRIS_UI

#include "lib.h"
#include "button.h"
#include "raylib.h"

constexpr Rectangle TILESET = { 0,0,16.0f,16.0f };
constexpr Rectangle TITLE = { 0,0,48.0f,16.0f };
constexpr float TILE_RATIO = 7.5f;
constexpr float TITLE_SIZE = 125.0f;

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
    gameStage getStage();
    Rectangle* getTetrisStage();
    float* getElapsedTime();
    bool quitGame();
    bool newGame();
private:
    void ShaderInit();
    // Scenes
    void TileSet();
    void TitleScreen();
    void Game(bool showText);
    void GameOver();

    // Stage
    gameStage _stage;
    float* _elapsedPtr;

    // Compositions
    Rectangle _Rect_tetrisStage;
    tetrisButton _Btn_restart;
    tetrisButton _Btn_quit;
    tetrisButton _Btn_Start;
    tetrisButton _Btn_Settings;
    tetrisButton _Btn_Exit;

    // Textures
    Texture2D _Texture_button;
    Texture2D _Texture_tileset_w;
    Texture2D _Texture_settings_w;
    Texture2D _Texture_logo;

    // Shaders
    Shader _Shader_blur;

    // UI States
    bool _exit;
    bool _newGame;
};

#endif // HEADER_TETRIS_UI