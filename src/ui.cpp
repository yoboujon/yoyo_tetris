#include "ui.h"
#include "button.h"
#include "lib.h"
#include "raylib.h"

tetrisUI::tetrisUI(float* elapsedPtr)
    : _stage(gameStage::TITLE_SCREEN)
    , _elapsedPtr(elapsedPtr) 
    , _Rect_tetrisStage({ 250, 40, 300, 450 })
    , _exit(false)
    , _newGame(false)
{
    // Init Textures
    _Texture_button = LoadTexture("res/base_button.png"); // Load button texture
    _Texture_tileset_w = LoadTexture("res/tileset_w.png");
    _Texture_settings_w = LoadTexture("res/tileset_b.png");
    _Texture_logo = LoadTexture("res/yoyotetris.png");
    // Init UI Objects
    _Btn_Start = tetrisButton(&_Texture_button, { 20, 180 }, ButtonStyle::NONE);
    _Btn_Start.SetText("Start");
    _Btn_Settings = tetrisButton(&_Texture_button, { 20, 240 }, ButtonStyle::NONE);
    _Btn_Settings.SetText("Settings");
    _Btn_Exit = tetrisButton(&_Texture_button, { 20, 300 }, ButtonStyle::NONE);
    _Btn_Exit.SetText("Exit");
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
    UnloadTexture(_Texture_logo);
}

void tetrisUI::Display(renderLayer layer)
{
    switch (_stage) {
    case gameStage::TITLE_SCREEN:
        if(layer == renderLayer::BACK)
        {
            TileSet();
            TitleScreen();
        }
        break;
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
    float ratio = 5.0f;
    //Offseting the texture by 5px in x, and 5px in y. (ratio multiplied by the actual pixels of the texture)
    DrawTextureRatio(_Texture_tileset_w, {-(ratio*5.0f),-(ratio*5.0f),SCREEN_WIDTH+(ratio*5.0f),SCREEN_HEIGHT+(ratio*5.0f)}, 5.0f, {0.0f,0.0f}, WHITE);
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {255,255,255,128});
}

void tetrisUI::TitleScreen()
{
    DrawTexturePro(_Texture_logo, TITLE, {0.0f,0.0f,TITLE_SIZE*3,TITLE_SIZE}, {0.0f,-10.0f}, 0.0f, WHITE);
    DrawText("Version 0.1", 20, 130, 20, BLACK_TEXT);

    _Btn_Start.Update();
    if (_Btn_Start.Clicked())
        _stage = gameStage::GAME;

    _Btn_Settings.Update();

    _Btn_Exit.Update();
    if (_Btn_Exit.Clicked())
        _exit = true;
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

gameStage tetrisUI::getStage() { return _stage; }
Rectangle* tetrisUI::getTetrisStage() { return &_Rect_tetrisStage; }
float* tetrisUI::getElapsedTime() { return _elapsedPtr; }
bool tetrisUI::quitGame() { return _exit; }
bool tetrisUI::newGame()
{
    bool tempNewGame(_newGame);
    _newGame = false;
    return tempNewGame;
}