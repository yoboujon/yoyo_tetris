#include "event/gameevent.h"
#include <iostream>
GameEvent::GameEvent(tetrisUI *ui, tetrisScore *score, tetrisGame *game)
    : _tetrisUI(ui), _tetrisScore(score), _tetrisGame(game), _staticBlocks(game->getStaticBlock())
{
    _tetrisUI->setEventHandler(this);
    _tetrisScore->setEventHandler(this);
    _tetrisGame->setEventHandler(this);
    _staticBlocks->setEventHandler(this);
}

GameEvent::~GameEvent()
{
}

void GameEvent::sendEvent(BaseComponent *sender, EventType type, const std::any &data)
{
    if(sender == _tetrisUI)
        uIEvents(type,data);
    if(sender == _tetrisScore)
        scoreEvents(type,data);
    if(sender == _tetrisGame)
        gameEvents(type,data);
    if(sender == _staticBlocks)
        staticBlockEvents(type,data);
    if(sender == nullptr)
        mainEvents(type,data);
}

void GameEvent::uIEvents(EventType type, const std::any& data)
{
    if (type == START_GAME)
        _tetrisGame->setTetrominoTexture(_tetrisUI->getTetrominoTexture());
    if (type == MENU_CLOSED)
        _tetrisGame->setPause(false);
}

void GameEvent::scoreEvents(EventType type, const std::any& data)
{
    if ((type == SEND_SCORE) && (data.type() == typeid(uint64_t)))
        _tetrisUI->setScore(std::any_cast<uint64_t>(data));
    if ((type == SEND_MULTIPLICATOR) && (data.type() == typeid(uint8_t)))
        _tetrisUI->setMultiplicator(std::any_cast<uint8_t>(data));
}

void GameEvent::gameEvents(EventType type, const std::any& data)
{
    if ((_tetrisUI->getStage() == gameStage::MENU_SCREEN) && (type == MENU_CLOSED))
        _tetrisUI->ChangeStage(gameStage::GAME);
}

void GameEvent::staticBlockEvents(EventType type, const std::any& data)
{
    if (type == TETRIS_LINE_COMPLETED)
    {
        _tetrisScore->lineComplete();
    }
}

void GameEvent::mainEvents(EventType type, const std::any& data)
{
        if (type == CREATED_NEW_GAME)
    {
        _staticBlocks->setEventHandler(this);
    }
}