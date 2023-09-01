#ifndef HEADER_TETRIS_COMPONENT
#define HEADER_TETRIS_COMPONENT

#include <any>

enum EventType
{
    NONE = 0,
    OPEN_MENU,
    BUTTON_PRESSED_CLOSE_MENU,
    ESCAPE_PRESSED_CLOSE_MENU,
    TETRIS_LINE_COMPLETED,
    SEND_SCORE,
    SEND_MULTIPLICATOR,
    GAME_OVER,
    START_GAME,
    CREATED_NEW_GAME
};

class BaseComponent;
class EventHandler
{
public:
    virtual void sendEvent(BaseComponent *sender, EventType type, const std::any &data = std::any()) = 0;
};

class BaseComponent
{
public:
    BaseComponent(EventHandler *event = nullptr);
    void setEventHandler(EventHandler *event);

protected:
    EventHandler *_eventHandler;
};

#endif // HEADER_TETRIS_COMPONENT