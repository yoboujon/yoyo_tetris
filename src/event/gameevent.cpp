#include "event/gameevent.h"
#include <iostream>
GameEvent::GameEvent(tetrisUI* ui, tetrisScore* score, tetrisGame* game, float *elapstedPtr)
    : _tetrisUI(ui), _tetrisScore(score), _tetrisGame(game), _staticBlocks(game->getStaticBlock()), _renderer(TetrisRenderer::GetInstance()), _elapsedPtr(elapstedPtr)
{
    _tetrisUI->setEventHandler(this);
    _tetrisScore->setEventHandler(this);
    _tetrisGame->setEventHandler(this);
    _staticBlocks->setEventHandler(this);
    _renderer.setEventHandler(this);
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
}

void GameEvent::uIEvents(EventType type, const std::any &data)
{
    // When Starting the game, loading the textures for tetrominos
    if (type == START_GAME)
        _tetrisGame->setTetrominoTexture(_renderer.GetTexture(textureId::TETROMINO_TILEMAP));
    // If the user prompted 'Resume' or 'Restart' buttons, the pause status is set to false.
    if (type == BUTTON_PRESSED_CLOSE_MENU)
        _tetrisGame->setPause(false);

    // When reseting the game (restart, exit, etc...)
    if (type == NEW_GAME)
    {
        _tetrisScore->resetScore();
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
    // Score gathering
    if ((type == SEND_SCORE) && (data.type() == typeid(uint64_t)))
        _tetrisUI->setScore(std::any_cast<uint64_t>(data));
    if ((type == SEND_MULTIPLICATOR) && (data.type() == typeid(uint8_t)))
        _tetrisUI->setMultiplicator(std::any_cast<uint8_t>(data));
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
    if(type == STAGE_CHANGED)
        _tetrisUI->RenderTile();
}