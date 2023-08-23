#include "ui.h"

#include <string>

tetrisUI::tetrisUI(tetrisEvent* event, float* elapsedPtr)
    : _versionNumber("Version " + std::string(VERSION_MAJOR) + "." + std::string(VERSION_MINOR) + "." + std::string(VERSION_PATCH))
    , _eventPtr(event)
    , _stage(gameStage::TITLE_SCREEN)
    , _elapsedPtr(elapsedPtr)
    , _Rect_tetrisStage(TETRIS_STAGE)
    , _shader_buffer({false,false})
    , _score(0)
    , _multiplicator(0)
    , _exit(false)
    , _newGame(false)
//, _kotoPiege(0.0f)
{
    // Init Textures
    _Texture_button = LoadTexture("res/button.png"); // Load button texture
    _Texture_playButton = LoadTexture("res/button_play.png");
    _Texture_settingButton = LoadTexture("res/button_settings.png");
    _Texture_exitButton = LoadTexture("res/button_exit.png");
    _Texture_tileset_w = LoadTexture("res/tileset_w.png");
    _Texture_tileset_b_borderless = LoadTexture("res/tileset_b_borderless.png");
    _Texture_tileset_b = LoadTexture("res/tileset_b.png");
    _Texture_logo = LoadTexture("res/yoyotetris.png");
    _Texture_tetromino = LoadTexture("res/tetromino.png");

    ShaderInit();

    // Init UI Objects
    _Btn_Start = tetrisButton(&_Texture_playButton, { 20, 180 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Start.SetText("Start");
    _Btn_Settings = tetrisButton(&_Texture_settingButton, { 20, 180 + 90.0f + 10 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Settings.SetText("Settings");
    _Btn_Exit = tetrisButton(&_Texture_exitButton, { 20, 180 + (90.0f + 10) * 2 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Exit.SetText("Exit");

    const Size2 menuSize = { 160.0f, 50.f };
    const float menuTotalWidth = tetrisButtongetTotalWidth(menuSize);
    _menuCenter = { (SCREEN_WIDTH - menuTotalWidth) / 2, (SCREEN_HEIGHT - menuSize.height) / 2 };
    _Btn_resume = tetrisButton(&_Texture_button, Vector2Add(_menuCenter, OFFSET_MENU(0)), menuSize);
    _Btn_resume.SetText("Resume");
    _Btn_restart = tetrisButton(&_Texture_button, Vector2Add(_menuCenter, OFFSET_MENU(1)), menuSize);
    _Btn_restart.SetText("Restart");
    _Btn_titleScreen = tetrisButton(&_Texture_button, Vector2Add(_menuCenter, OFFSET_MENU(2)), menuSize);
    _Btn_titleScreen.SetText("Title Screen");

    // Target textures
    _back = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _texture_buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
}

tetrisUI::~tetrisUI()
{
    UnloadTexture(_Texture_button);
    UnloadTexture(_Texture_playButton);
    UnloadTexture(_Texture_tileset_w);
    UnloadTexture(_Texture_settings_w);
    UnloadTexture(_Texture_logo);
    UnloadTexture(_Texture_tetromino);
    UnloadShader(_Shader_blur);
    UnloadRenderTexture(_back);
    UnloadRenderTexture(_front);
    UnloadRenderTexture(_texture_buffer);
}

void tetrisUI::ShaderInit()
{
    // blur radius
    const int radius = 8;
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
            Game();
        }
        break;
    case gameStage::GAME_OVER:
        if (layer == renderLayer::BACK) {
            TileSet();
            Game();
        } else
            GameOver();
        break;
    case gameStage::MENU_SCREEN:
        if (layer == renderLayer::FRONT)
            MenuScreen();
    default:
        break;
    }

    // Events to update variables
    uint64_t tempScore;
    uint8_t tempMultiplicator;
    if (_eventPtr->OnEvent(eventType::SEND_SCORE, eventUser::UI, tempScore))
        _score = tempScore;
    if (_eventPtr->OnEvent(eventType::SEND_MULTIPLICATOR, eventUser::UI, tempMultiplicator))
        _multiplicator = tempMultiplicator;
}

void tetrisUI::DisplayTexture()
{
    auto backPtr = &_back;
    auto frontPtr = &_front;

    switch (_stage) {
    case gameStage::MENU_SCREEN:
    case gameStage::GAME_OVER:
        // Shader optimisation : creating a texture buffer everytime the scene is called
        // The 'BeginShaderMode' in this exemple is only called once.
        if (!_shader_buffer[0]) {
            BeginTextureMode(_texture_buffer);
            BeginShaderMode(_Shader_blur);
            DrawTextureRec(_back.texture, { 0, 0, (float)_back.texture.width, (float)-_back.texture.height }, { 0, 0 }, WHITE);
            EndShaderMode();
            EndTextureMode();
            _shader_buffer[0] = true;
        }
        // We set the back pointer texture to the shader buffer created.
        backPtr = &_texture_buffer;
        break;
    default:
        for(auto& shader : _shader_buffer)
            shader = false;
        break;
    }

    // Begin to draw the scene
    BeginDrawing();
    // minus the height because the texture height is inverted in the RenderTexture2D.
    DrawTextureRec(backPtr->texture, { 0, 0, (float)backPtr->texture.width, (float)-backPtr->texture.height }, { 0, 0 }, WHITE);
    DrawTextureRec(frontPtr->texture, { 0, 0, (float)frontPtr->texture.width, (float)-frontPtr->texture.height }, { 0, 0 }, WHITE);
    EndDrawing();
}

void tetrisUI::TileSet()
{
    // Offseting the texture by 5px in x, and 5px in y. (ratio multiplied by the actual pixels of the texture)
    DrawTextureRatio(_Texture_tileset_w, { 0.0f, 0.0f }, TILE_DESTINATION, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 255, 255, 255, 128 });
}

void tetrisUI::TitleScreen()
{
    DrawTexturePro(_Texture_logo, TITLE, { 0.0f, 0.0f, TITLE_SIZE * 3, TITLE_SIZE }, { 0.0f, -10.0f }, 0.0f, WHITE);
    DrawText(_versionNumber.c_str(), 20, 130, 20, BLACK_TEXT);

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
    // TODO : Write all the elements of the game depending on the number of tiles.
    // This way Even in other resolutions the game should be designed the same way.

    Rectangle UIrectangle = { (_Rect_tetrisStage.x) - TILE_RATIO, (_Rect_tetrisStage.y) - TILE_RATIO, (_Rect_tetrisStage.width) + TILE_RATIO * 2, (_Rect_tetrisStage.height) + TILE_RATIO * 2 };
    DrawText("Score:", static_cast<int>(TILE_RATIO * 9), static_cast<int>(TILE_RATIO * 9), 30, DARKGRAY);
    DrawText(std::to_string(_score).c_str(), static_cast<int>(TILE_RATIO * 9), static_cast<int>(TILE_RATIO * 17), 30, DARKGRAY);
    DrawText("Next:", static_cast<int>(TILE_RATIO * 97), static_cast<int>(TILE_RATIO * 9), 30, DARKGRAY);
    if (_multiplicator > 1)
        DrawText(("Multiplicator: x" + std::to_string(_multiplicator)).c_str(), static_cast<int>(TILE_RATIO * 9), static_cast<int>(TILE_RATIO * 25), 20, DARKGRAY);
    // x=3 and y=4 for the offset because of the actual texture.
    // 3*7+4 = 24 -> 3 Tiles + 4 border -> What we want to draw for the next
    DrawTextureRatio(_Texture_tileset_b, { 3.0f, 4.0f }, { 488, 118, 25 * TILE_RATIO, 25 * TILE_RATIO }, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    DrawTextureRatio(_Texture_tileset_b, { 3.0f, 4.0f }, UIrectangle, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    // ! 3.8f is for now a dark magic number, it seems to be linked with the 4.0f offset of the texture, but why 0.2 ?
    DrawTextureRatio(_Texture_tileset_b_borderless, { 3.0f, 3.8f }, _Rect_tetrisStage, TETRIS_STAGE.width / (CASE_NUM * 8), { 0.0f, 0.0f }, WHITE);
}

void tetrisUI::GameOver()
{
    // Const
    const int gameOvertext = MeasureText("Game Over", 30);

    // Updating Buttons position
    _Btn_restart.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(0)));
    _Btn_titleScreen.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(1)));

    // Drawing UI
    DrawRectangleRec({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, PAUSE_COLOR);
    DrawText("Game Over", (SCREEN_WIDTH - gameOvertext) / 2, (SCREEN_HEIGHT / 2) - 150, 30, RAYWHITE);
    const std::string scoreText = "Final score: " + std::to_string(_score);
    DrawText(scoreText.c_str(), (SCREEN_WIDTH - MeasureText(scoreText.c_str(), 20)) / 2, (SCREEN_HEIGHT / 2) - 100, 20, GOLD);

    // Button Actions
    _Btn_titleScreen.Update();
    if (_Btn_titleScreen.Clicked()) {
        _stage = gameStage::TITLE_SCREEN;
        _newGame = true;
        // Unloading render texture and reloading a clear buffer.
        UnloadRenderTexture(_front);
        _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    _Btn_restart.Update();
    if (_Btn_restart.Clicked()) {
        _newGame = true;
        UnloadRenderTexture(_front);
        _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void tetrisUI::MenuScreen()
{
    // Const
    const int menuText = MeasureText("Menu", 30);

    _Btn_resume.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(0)));
    _Btn_restart.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(1)));
    _Btn_titleScreen.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(2)));

    DrawRectangleRec({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, PAUSE_COLOR);
    DrawText("Menu", (SCREEN_WIDTH - menuText) / 2, (SCREEN_HEIGHT / 2) - 100, 30, RAYWHITE);

    if (_eventPtr->OnEvent(eventType::MENU_CLOSED, eventUser::UI)) {
        _stage = gameStage::GAME;
        UnloadRenderTexture(_front);
        _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    _Btn_resume.Update();
    if (_Btn_resume.Clicked()) {
        _eventPtr->callEvent(eventType::MENU_CLOSED, eventUser::TETRIS);
        _stage = gameStage::GAME;
        UnloadRenderTexture(_front);
        _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    _Btn_titleScreen.Update();
    if (_Btn_titleScreen.Clicked()) {
        _stage = gameStage::TITLE_SCREEN;
        _newGame = true;
        // Unloading render texture and reloading a clear buffer.
        UnloadRenderTexture(_front);
        _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    _Btn_restart.Update();
    if (_Btn_restart.Clicked()) {
        _newGame = true;
        UnloadRenderTexture(_front);
        _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

// Setters

void tetrisUI::ChangeStage(gameStage stage) { _stage = stage; }

// Getters

gameStage tetrisUI::getStage() { return _stage; }
Rectangle* tetrisUI::getTetrisStage() { return &_Rect_tetrisStage; }
float* tetrisUI::getElapsedTime() { return _elapsedPtr; }
Shader* tetrisUI::getShaderBlur() { return &_Shader_blur; }
RenderTexture2D* tetrisUI::getRenderTexture(renderLayer layer) { return (layer == renderLayer::BACK ? &_back : &_front); }
Texture2D* tetrisUI::getTetrominoTexture() { return &_Texture_tetromino; }
bool tetrisUI::quitGame() { return _exit; }
bool tetrisUI::newGame()
{
    bool tempNewGame(_newGame);
    _newGame = false;
    return tempNewGame;
}