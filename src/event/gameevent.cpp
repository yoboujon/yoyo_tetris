#include "event/gameevent.h"
#include "event/component.h"
#include "lib.h"
#include <cstddef>
#include <iostream>

GameEvent::GameEvent(tetrisUI* ui, tetrisScore* score, tetrisGame* game, float *elapstedPtr)
    : _tetrisUI(ui), _tetrisScore(score), _tetrisGame(game), _staticBlocks(game->getStaticBlock()), _renderer(TetrisRenderer::GetInstance()), _inputManager(TetrisInputManager::GetInstance()), _elapsedPtr(elapstedPtr), _loading(false)
{
    _tetrisUI->setEventHandler(this);
    _tetrisScore->setEventHandler(this);
    _tetrisGame->setEventHandler(this);
    _staticBlocks->setEventHandler(this);
    _renderer.setEventHandler(this);
    _inputManager.setEventHandler(this);
}

GameEvent::~GameEvent()
{
}

void GameEvent::sendEvent(BaseComponent *sender, EventType type, const std::any &data)
{
    if (sender == _tetrisUI)
        uIEvents(type, data);
    if (sender == _tetrisScore)
        scoreEvents(type, data);
    if (sender == _tetrisGame)
        gameEvents(type, data);
    if (sender == _staticBlocks)
        staticBlockEvents(type, data);
    if (sender == &_renderer)
        rendererEvents(type, data);
    if (sender == &_inputManager)
        inputEvents(type, data);
    if (sender == nullptr)
        mainEvents(type, data);
}

void GameEvent::uIEvents(EventType type, const std::any &data)
{
    // If the user prompted 'Resume' or 'Restart' buttons, the pause status is set to false.
    if (type == BUTTON_PRESSED_CLOSE_MENU)
        _tetrisGame->setPause(false);

    // When reseting the game (restart, exit, etc...)
    if (type == NEW_GAME)
    {
        // Reseting the score and the ui elements
        _tetrisScore->resetScore(true);
        _tetrisGame->reset(_elapsedPtr);
        // Loading the textures only if we stay on stages that needs these textures.
        // As a matter of fact, when starting a game from the titlescreen : this function will be called again.
        if (_renderer.GetStage() != gameStage::TITLE_SCREEN)
            _tetrisGame->setTetrominoTexture(_renderer.GetTexture(textureId::TETROMINO_TILEMAP));
        // When a new game is created, the static bloc is reset
        // We have to make sure the event handler is set.
        _staticBlocks->setEventHandler(this);
    }
}

void GameEvent::scoreEvents(EventType type, const std::any &data)
{
    // Score gathering && render tileset.
    if ((type == SEND_SCORE) && (data.type() == typeid(uint64_t)))
    {
        _tetrisUI->setScore(std::any_cast<uint64_t>(data));
        _tetrisUI->RenderTile();
    }
    if ((type == SEND_MULTIPLICATOR) && (data.type() == typeid(uint8_t)))
    {
        _tetrisUI->setMultiplicator(std::any_cast<uint8_t>(data));
        _tetrisUI->RenderTile();
    }
}

void GameEvent::gameEvents(EventType type, const std::any &data)
{
    // ESCAPE is pressed and we were in game.
    if (type == OPEN_MENU)
        _renderer.ChangeStage(gameStage::MENU_SCREEN);
    // ESCAPE is pressed and we were in a menu.
    if ((_renderer.GetStage() == gameStage::MENU_SCREEN) && (type == ESCAPE_PRESSED_CLOSE_MENU))
        _renderer.ChangeStage(gameStage::GAME);

    // GAMEOVER
    if (type == GAME_OVER)
        _renderer.ChangeStage(gameStage::GAME_OVER);
}

void GameEvent::staticBlockEvents(EventType type, const std::any &data)
{
    // When the static block send the event TETRIS_LINE_COMPLETED
    // the tetris score is informed to update the score.
    if (type == TETRIS_LINE_COMPLETED)
        _tetrisScore->lineComplete();
}

void GameEvent::rendererEvents(EventType type, const std::any &data)
{
    auto actualStage = _renderer.GetStage();

    // Set loading to true. No rendering has to occur until the textures are fully loaded
    if(type == CHANGING_STAGE)
    {
        _loading = true;
        _changingStage = std::any_cast<gameStage>(data);
    }
    // If it's loading and finished display loading, we can load the textures.
    if((type == END_DISPLAY_LOADING) && _loading)
        _renderer.UpdateTexturesStage(_changingStage);
    // Textures are loaded, we can display the new scene
    if(type == TEXTURES_LOADED)
    {
        _loading = false;
        // Rendering tiles each time we change scene and textures are loaded.
        _tetrisUI->RenderTile();
        TetrisMouse::GetInstance().ResetMouse();
        if(IsCursorHidden())
            EnableCursor();
        if (actualStage == gameStage::GAME)
        {
            DisableCursor();
            _tetrisGame->setTetrominoTexture(_renderer.GetTexture(textureId::TETROMINO_TILEMAP));
            _tetrisGame->StartGame();
        }
    }
}

void GameEvent::inputEvents(EventType type, const std::any& data)
{
    // When a key is pressed, update the tile.
    if(type == KEY_PRESSED)
        _tetrisUI->RenderTile();
}

void GameEvent::mainEvents(EventType type, const std::any& data)
{
    // When in a menu; no need to render the tiles
    bool inMenu = (_renderer.GetStage() == gameStage::MENU_SCREEN) || (_renderer.GetStage() == gameStage::GAME_OVER);
    // When the user move his mouse, the tileset has to be rendered.
    if((type == MOUSE_MOVED) && !inMenu)
        _tetrisUI->RenderTile();
    if((type == LEFT_CLICK) && !inMenu)
        _tetrisUI->RenderTile();
    if((type == KEY_PRESSED) && !inMenu)
        _tetrisUI->RenderTile();
}

bool GameEvent::getLoading() { return _loading; }