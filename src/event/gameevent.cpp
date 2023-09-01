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
    if(type == EventType::TETRIS_LINE_COMPLETED)
        _tetrisScore->lineComplete();

    if(type == EventType::START_GAME)
        _tetrisGame->setTetrominoTexture(_tetrisUI->getTetrominoTexture());
    if (type == EventType::MENU_CLOSED)
        _tetrisGame->setPause(false);
    
    if((_tetrisUI->getStage() == gameStage::MENU_SCREEN) && (type == EventType::MENU_CLOSED))
        _tetrisUI->ChangeStage(gameStage::GAME);
}