#ifndef HEADER_TETRIS_GAME_EVENT
#define HEADER_TETRIS_GAME_EVENT

#include "event/component.h"
#include "graphics/ui.h"
#include "game/tetris.h"
#include "game/score.h"

class GameEvent : public EventHandler
{
public:
    GameEvent(tetrisUI* ui, tetrisScore* score, tetrisGame* game);
    ~GameEvent();
    void sendEvent(BaseComponent* sender, EventType type, const std::any& data = std::any());
private:
    void uIEvents(EventType type, const std::any& data = std::any());
    void scoreEvents(EventType type, const std::any& data = std::any());
    void gameEvents(EventType type, const std::any& data = std::any());
    void staticBlockEvents(EventType type, const std::any& data = std::any());
    void mainEvents(EventType type, const std::any& data = std::any());

    tetrisUI* _tetrisUI;
    tetrisScore* _tetrisScore;
    tetrisGame* _tetrisGame;
    tetrisStaticBlocks* _staticBlocks;
};

#endif //HEADER_TETRIS_GAME_EVENT