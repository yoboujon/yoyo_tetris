#include "event/gameevent.h"

GameEvent::GameEvent(tetrisUI* ui, tetrisScore* score, tetrisGame* game)
    : _tetrisUI(ui)
    , _tetrisScore(score)
    , _tetrisGame(game)
    , _staticBlocks(game->getStaticBlock())
{
    _tetrisUI->setEventHandler(this);
    _tetrisScore->setEventHandler(this);
    _tetrisGame->setEventHandler(this);
    _staticBlocks->setEventHandler(this);
}

GameEvent::~GameEvent()
{
}

void GameEvent::sendEvent(BaseComponent* sender, EventType type) const
{
    //Comes from staticBlock
    if(type == TETRIS_LINE_COMPLETED)
    {
        _tetrisScore->lineComplete();
    }

    //Comes from tetrisUI
    if(type == START_GAME)
        _tetrisGame->setTetrominoTexture(_tetrisUI->getTetrominoTexture());
    if (type == MENU_CLOSED)
        _tetrisGame->setPause(false);
    
    //Comes from tetrisGame
    if((_tetrisUI->getStage() == gameStage::MENU_SCREEN) && (type == MENU_CLOSED))
        _tetrisUI->ChangeStage(gameStage::GAME);

    //Comes from main
    if(type == CREATED_NEW_GAME)
    {
        GameEvent* eventHandlerTemp = const_cast<GameEvent*>(this);
        _staticBlocks->setEventHandler(eventHandlerTemp);
    }
}

void GameEvent::sendEvent(BaseComponent* sender, EventType type, const std::any& data) const
{
    if ((type == SEND_SCORE) && (data.type() == typeid(uint64_t)))
        _tetrisUI->setScore(std::any_cast<uint64_t>(data));
    if ((type == SEND_MULTIPLICATOR) && (data.type() == typeid(uint8_t)))
        _tetrisUI->setMultiplicator(std::any_cast<uint8_t>(data));
}