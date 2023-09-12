#include "graphics/ui.h"
#include "graphics/render.h"

#include <string>
#include <iostream>

tetrisUI::tetrisUI()
    : _versionNumber("Version " + std::string(VERSION_MAJOR) + "." + std::string(VERSION_MINOR) + "." + std::string(VERSION_PATCH))
    , _score(0)
    , _multiplicator(0)
    , _Rect_tetrisStage(TETRIS_STAGE)
    , _textureLoader(TetrisRenderer::GetInstance().GetTextureLoader())
    , _exit(false)
    , _showTiles(true)
//, _kotoPiege(0.0f)
{
    // Init Textures
    _textureLoader.load(textureId::TILESET_WHITE,unloadState::NEVER);

    // Init UI Objects
    // TitleScreen Buttons
    _Btn_Start = tetrisButton({ 20, 180 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Start.SetText("Start");
    _Btn_Settings = tetrisButton({ 20, 180 + 90.0f + 10 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Settings.SetText("Settings");
    _Btn_Exit = tetrisButton({ 20, 180 + (90.0f + 10) * 2 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Exit.SetText("Exit");

    // Settings Buttons
    _Btn_Back = tetrisButton({ 20, 180 + (90.0f + 10) * 2 }, { 160.0f, 90.0f }, textureStyle::CUSTOM_SHAPE, { 32.0f, 18.0f });
    _Btn_Back.SetText("Back");
    _TxtInp_FrameRate = TetrisInputText({20, 180}, { 100.0f, 50.0f });

    // Menu Buttons
    const Size2 menuSize = { 160.0f, 50.f };
    const float menuTotalWidth = tetrisButtongetTotalWidth(menuSize);
    _menuCenter = { (SCREEN_WIDTH - menuTotalWidth) / 2, (SCREEN_HEIGHT - menuSize.height) / 2 };
    _Btn_resume = tetrisButton(Vector2Add(_menuCenter, OFFSET_MENU(0)), menuSize);
    _Btn_resume.SetText("Resume");
    _Btn_restart = tetrisButton(Vector2Add(_menuCenter, OFFSET_MENU(1)), menuSize);
    _Btn_restart.SetText("Restart");
    _Btn_titleScreen = tetrisButton(Vector2Add(_menuCenter, OFFSET_MENU(2)), menuSize);
    _Btn_titleScreen.SetText("Title Screen");
}

tetrisUI::~tetrisUI()
{
}

void tetrisUI::Display(RendererLayer layer)
{
    // Gathering actual stage
    auto stage = TetrisRenderer::GetInstance().GetStage();
    // Updating tiles only when asked by the event.
    if(_showTiles && layer == RendererLayer::BACK)
    {
        _showTiles = false;
        TileSet(textureFromStage(stage),colorFromStage(stage));
    }
    //if(layer == RendererLayer::BACK)  

    // Displaying depending on the stage
    switch (stage) {
    case gameStage::TITLE_SCREEN:
        if (layer == RendererLayer::BACK)
            TitleScreen();
        break;
    case gameStage::GAME:
        if (layer == RendererLayer::BACK)
            Game();
        break;
    case gameStage::GAME_OVER:
        if (layer == RendererLayer::FRONT)
            GameOver();
        break;
    case gameStage::MENU_SCREEN:
        if (layer == RendererLayer::FRONT)
            MenuScreen();
        break;
    case gameStage::SETTINGS:
        if(layer == RendererLayer::BACK)
            Settings();
        break;
    default:
        break;
    }
}

void tetrisUI::TileSet(textureId texture, Color backColor)
{
    auto stage = TetrisRenderer::GetInstance().GetStage();
    if(stage == gameStage::GAME_OVER || stage == gameStage::MENU_SCREEN)
        return;
    // Offseting the texture by 5px in x, and 5px in y. (ratio multiplied by the actual pixels of the texture)
    auto tileSetTexture = _textureLoader.getTexture(texture);
    DrawTextureRatio(tileSetTexture, { 0.0f, 0.0f }, TILE_DESTINATION, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backColor);
}

void tetrisUI::TitleScreen()
{
    auto logoTexture = _textureLoader.getTexture(textureId::LOGO);
    DrawTexturePro(logoTexture, TITLE, { 0.0f, 0.0f, TITLE_SIZE * 3, TITLE_SIZE }, { 0.0f, -10.0f }, 0.0f, WHITE);
    DrawText(_versionNumber.c_str(), 20, 130, 20, BLACK_TEXT);

    auto buttonPlay = _textureLoader.getTexture(textureId::BUTTON_PLAY);
    auto buttonSettings = _textureLoader.getTexture(textureId::BUTTON_SETTINGS);
    auto buttonExit = _textureLoader.getTexture(textureId::BUTTON_EXIT);

    _Btn_Start.Update(buttonPlay);
    if (_Btn_Start.Clicked())
    {
        TetrisRenderer::GetInstance().ChangeStage(gameStage::GAME);
        this->_eventHandler->sendEvent(this, EventType::START_GAME);
    }

    _Btn_Settings.Update(buttonSettings);
    if (_Btn_Settings.Clicked())
    {
        TetrisRenderer::GetInstance().ChangeStage(gameStage::SETTINGS);
    }

    _Btn_Exit.Update(buttonExit);
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
    auto blackTileset = _textureLoader.getTexture(textureId::TILESET_BLACK);
    auto blackTilesetBorderless = _textureLoader.getTexture(textureId::TILESET_BLACK_BORDERLESS);
    DrawTextureRatio(blackTileset, { 3.0f, 4.0f }, { 488, 118, 25 * TILE_RATIO, 25 * TILE_RATIO }, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    DrawTextureRatio(blackTileset, { 3.0f, 4.0f }, UIrectangle, TILE_RATIO, { 0.0f, 0.0f }, WHITE);
    // ! 3.8f is for now a dark magic number, it seems to be linked with the 4.0f offset of the texture, but why 0.2 ?
    DrawTextureRatio(blackTilesetBorderless, { 3.0f, 3.8f }, _Rect_tetrisStage, TETRIS_STAGE.width / (CASE_NUM * 8), { 0.0f, 0.0f }, WHITE);
}

void tetrisUI::GameOver()
{
    // Const
    const int gameOvertext = MeasureText("Game Over", 30);

    // Updating Buttons position
    _Btn_restart.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(0)));
    _Btn_titleScreen.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(1)));

    // Drawing UI
    DrawText("Game Over", (SCREEN_WIDTH - gameOvertext) / 2, (SCREEN_HEIGHT / 2) - 150, 30, RAYWHITE);
    const std::string scoreText = "Final score: " + std::to_string(_score);
    DrawText(scoreText.c_str(), (SCREEN_WIDTH - MeasureText(scoreText.c_str(), 20)) / 2, (SCREEN_HEIGHT / 2) - 100, 20, GOLD);

    auto buttonBase = _textureLoader.getTexture(textureId::BUTTON_BASE);

    // Button Actions
    _Btn_titleScreen.Update(buttonBase);
    if (_Btn_titleScreen.Clicked()) {
        this->_eventHandler->sendEvent(this, NEW_GAME);
        TetrisRenderer::GetInstance().ChangeStage(gameStage::TITLE_SCREEN);
    }

    _Btn_restart.Update(buttonBase);
    if (_Btn_restart.Clicked()) {
        this->_eventHandler->sendEvent(this, NEW_GAME);
        TetrisRenderer::GetInstance().ChangeStage(gameStage::GAME);
    }
}

void tetrisUI::MenuScreen()
{
    // Const
    const int menuText = MeasureText("Menu", 30);

    _Btn_resume.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(0)));
    _Btn_restart.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(1)));
    _Btn_titleScreen.setPosition(Vector2Add(_menuCenter, OFFSET_MENU(2)));

    DrawText("Menu", (SCREEN_WIDTH - menuText) / 2, (SCREEN_HEIGHT / 2) - 100, 30, RAYWHITE);

    auto buttonBase = _textureLoader.getTexture(textureId::BUTTON_BASE);

    _Btn_resume.Update(buttonBase);
    if (_Btn_resume.Clicked()) {
        this->_eventHandler->sendEvent(this, EventType::BUTTON_PRESSED_CLOSE_MENU);
        TetrisRenderer::GetInstance().ChangeStage(gameStage::GAME);
    }

    _Btn_titleScreen.Update(buttonBase);
    if (_Btn_titleScreen.Clicked()) {
        this->_eventHandler->sendEvent(this, NEW_GAME);
        TetrisRenderer::GetInstance().ChangeStage(gameStage::TITLE_SCREEN);
    }

    _Btn_restart.Update(buttonBase);
    if (_Btn_restart.Clicked()) {
        this->_eventHandler->sendEvent(this, NEW_GAME);
        TetrisRenderer::GetInstance().ChangeStage(gameStage::GAME);
    }
}

void tetrisUI::Settings()
{
    auto buttonback = _textureLoader.getTexture(textureId::BUTTON_RETURN);
    std::string text;

    _TxtInp_FrameRate.Update(TetrisRenderer::GetInstance().GetTexture(textureId::TEXT_BOX));

    _Btn_Back.Update(buttonback);
    if(_Btn_Back.Clicked())
    {
        TetrisRenderer::GetInstance().ChangeStage(gameStage::TITLE_SCREEN);
    }
}

void tetrisUI::RenderTile()
{
    _showTiles = true;
}

textureId tetrisUI::textureFromStage(gameStage stage)
{
    if(stage == gameStage::SETTINGS)
        return textureId::TILESET_SETTINGS_WHITE;
    else
        return textureId::TILESET_WHITE;
}

Color tetrisUI::colorFromStage(gameStage stage)
{
    if(stage == gameStage::SETTINGS)
        return { 0, 0, 0, 128 };
    else
        return { 255, 255, 255, 128 };
}

// Setters
void tetrisUI::setScore(uint64_t score) { _score = score; }
void tetrisUI::setMultiplicator(uint8_t multiplicator) { _multiplicator = multiplicator; }

// Getters
bool tetrisUI::quitGame() { return _exit; }