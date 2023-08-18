#include "ui.h"
#include "button.h"
#include "lib.h"
#include "raylib.h"
#include <iostream>

tetrisUI::tetrisUI(float* elapsedPtr)
    : _stage(gameStage::TITLE_SCREEN)
    , _elapsedPtr(elapsedPtr)
    , _Rect_tetrisStage({ 250, 40, 300, 450 })
    , _exit(false)
    , _newGame(false)
//, _kotoPiege(0.0f)
{
    // Init Textures
    _Texture_button = LoadTexture("res/base_button.png"); // Load button texture
    _Texture_tileset_w = LoadTexture("res/tileset_w.png");
    _Texture_tileset_b = LoadTexture("res/tileset_b.png");
    _Texture_logo = LoadTexture("res/yoyotetris.png");

    ShaderInit();

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
    UnloadShader(_Shader_blur);
}

void tetrisUI::ShaderInit()
{
    // blur radius
    int radius = 8;
    _Shader_blur = LoadShader(0, "res/shaders/blur.fs");
    SetShaderValue(_Shader_blur, GetShaderLocation(_Shader_blur, "radius"), &radius, SHADER_UNIFORM_INT);
    SetShaderValue(_Shader_blur, GetShaderLocation(_Shader_blur, "width"), &SCREEN_WIDTH, SHADER_UNIFORM_INT);
    SetShaderValue(_Shader_blur, GetShaderLocation(_Shader_blur, "height"), &SCREEN_HEIGHT, SHADER_UNIFORM_INT);
}

void tetrisUI::Display(renderLayer layer)
{
    switch (_stage) {
    case gameStage::TITLE_SCREEN:
        if (layer == renderLayer::BACK) {
            TileSet();
            TitleScreen();
        }
        break;
    case gameStage::GAME:
        if (layer == renderLayer::BACK) {
            TileSet();
            Game(true);
        }
        break;
    case gameStage::GAME_OVER:
        if (layer == renderLayer::BACK) {
            TileSet();
            Game(false);
        }
        if (layer == renderLayer::FRONT)
            GameOver();
        break;
    default:
        break;
    }
}

void tetrisUI::DisplayShader(renderLayer layer, bool end)
{
    switch (_stage) {
    case gameStage::GAME_OVER:
        if (layer == renderLayer::BACK)
        {
            end ? EndShaderMode() : BeginShaderMode(_Shader_blur);
        }
        break;
    default:
        break;
    }
}

void tetrisUI::TileSet()
{
    // Offseting the texture by 5px in x, and 5px in y. (ratio multiplied by the actual pixels of the texture)
    DrawTextureRatio(_Texture_tileset_w, { 0.0f, 0.0f }, { -(TILE_RATIO * 5.0f), -(TILE_RATIO * 5.0f), SCREEN_WIDTH + (TILE_RATIO * 5.0f), SCREEN_HEIGHT + (TILE_RATIO * 5.0f) }, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 255, 255, 255, 128 });
}

void tetrisUI::TitleScreen()
{
    DrawTexturePro(_Texture_logo, TITLE, { 0.0f, 0.0f, TITLE_SIZE * 3, TITLE_SIZE }, { 0.0f, -10.0f }, 0.0f, WHITE);
    DrawText("Version 0.1", 20, 130, 20, BLACK_TEXT);

    _Btn_Start.Update();
    if (_Btn_Start.Clicked())
        _stage = gameStage::GAME;

    _Btn_Settings.Update();

    _Btn_Exit.Update();
    if (_Btn_Exit.Clicked())
        _exit = true;
}

void tetrisUI::Game(bool showText)
{
    Rectangle UIrectangle = { (_Rect_tetrisStage.x) - 4, (_Rect_tetrisStage.y) - 4, (_Rect_tetrisStage.width) + 8, (_Rect_tetrisStage.height) + 8 };
    if (showText) {
        DrawText("YoyoTetris", 40, 40, 30, DARKGRAY);
        DrawText("Score", 40, 120, 20, DARKGRAY);
        DrawText("Level", 40, 300, 20, DARKGRAY);
        DrawText("Next", 622, 70, 30, DARKGRAY);
    }
    DrawTextureRatio(_Texture_tileset_b, { 3.0f, 4.0f }, { 592, 112, NEXT_SIZE, NEXT_SIZE }, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    // DrawRectangleRec(UIrectangle, DARKGRAY);
    // DrawRectangleGradientEx(_Rect_tetrisStage, WHITE, LIGHTGRAY, LIGHTGRAY, WHITE);
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
    if (_Btn_restart.Clicked()) {
        _newGame = true;
    }
}

// Setters

void tetrisUI::ChangeStage(gameStage stage) { _stage = stage; }

// Getters

gameStage tetrisUI::getStage() { return _stage; }
Rectangle* tetrisUI::getTetrisStage() { return &_Rect_tetrisStage; }
float* tetrisUI::getElapsedTime() { return _elapsedPtr; }
Shader tetrisUI::getShaderBlur() { return _Shader_blur; }
bool tetrisUI::quitGame() { return _exit; }
bool tetrisUI::newGame()
{
    bool tempNewGame(_newGame);
    _newGame = false;
    return tempNewGame;
}