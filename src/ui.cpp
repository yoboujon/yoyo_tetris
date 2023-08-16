#include "ui.h"
#include "lib.h"
#include "raylib.h"

tetrisUI::tetrisUI(float* elapsedPtr)
    : _stage(gameStage::GAME)
    , _elapsedPtr(elapsedPtr) 
    , _Rect_tetrisStage({ 250, 40, 300, 450 })
    , _exit(false)
    , _newGame(false)
{
    // Init Textures
    _Texture_button = LoadTexture("res/base_button.png"); // Load button texture
    _Texture_tileset_w = LoadTexture("res/tileset_w.png");
    // Init UI Objects
    _Btn_restart = tetrisButton(&_Texture_button, { 0, 50 }, ButtonStyle::CENTERED);
    _Btn_restart.SetText("Restart");
    _Btn_quit = tetrisButton(&_Texture_button, { 0, 0 }, ButtonStyle::CENTERED);
    _Btn_quit.SetText("Quit");
}

tetrisUI::~tetrisUI()
{
    UnloadTexture(_Texture_button);
    UnloadTexture(_Texture_tileset_w);
    UnloadTexture(_Texture_settings_w);
}

void tetrisUI::Display(renderLayer layer)
{
    switch (_stage) {
    case gameStage::GAME:
        if (layer == renderLayer::BACK)
        {
            TileSet();
            Game();
        }
        break;
    case gameStage::GAME_OVER:
        if (layer == renderLayer::BACK)
        {
            TileSet();
            Game();
        }
        if (layer == renderLayer::FRONT)
            GameOver();
        break;
    default:
        break;
    }
}

void tetrisUI::TileSet()
{
    DrawTextureRatio(_Texture_tileset_w, {0,-1,SCREEN_WIDTH,SCREEN_HEIGHT}, 5.0f, NULL_VECTOR2, WHITE);
}

void tetrisUI::Game()
{
    Rectangle UIrectangle = { (_Rect_tetrisStage.x) - 4, (_Rect_tetrisStage.y) - 4, (_Rect_tetrisStage.width) + 8, (_Rect_tetrisStage.height) + 8 };
    DrawText("YoyoTetris", 40, 40, 30, DARKGRAY);
    DrawText("Score", 40, 120, 20, DARKGRAY);
    DrawText("Level", 40, 300, 20, DARKGRAY);
    DrawText("Next", 650, 40, 20, DARKGRAY);
    DrawRectangleRec(UIrectangle, DARKGRAY);
    DrawRectangleGradientEx(_Rect_tetrisStage, WHITE, LIGHTGRAY, LIGHTGRAY, WHITE);
}

void tetrisUI::GameOver()
{
    // Const
    const int gameOvertext = MeasureText("Game Over", 30);

    // Drawing UI
    DrawRectangleRec({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, PAUSE_COLOR);
    DrawText("Game Over", (SCREEN_WIDTH - gameOvertext) / 2, (SCREEN_HEIGHT / 2) - 100, 30, RAYWHITE);

    // Button Actions
    _Btn_quit.Update();
    if (_Btn_quit.Clicked())
        _exit = true;

    _Btn_restart.Update();
    if (_Btn_restart.Clicked())
        _newGame = true;
}

// Setters

void tetrisUI::ChangeStage(gameStage stage) { _stage = stage; }

// Getters

Rectangle* tetrisUI::getTetrisStage() { return &_Rect_tetrisStage; }
float* tetrisUI::getElapsedTime() { return _elapsedPtr; }
bool tetrisUI::quitGame() { return _exit; }
bool tetrisUI::newGame()
{
    bool tempNewGame(_newGame);
    _newGame = false;
    return tempNewGame;
}