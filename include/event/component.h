#ifndef HEADER_TETRIS_COMPONENT
#define HEADER_TETRIS_COMPONENT

#include <any>

enum EventType
{
    NONE = 0,
    CHANGING_STAGE,
    OPEN_MENU,
    BUTTON_PRESSED_CLOSE_MENU,
    ESCAPE_PRESSED_CLOSE_MENU,
    TETRIS_LINE_COMPLETED,
    SEND_SCORE,
    SEND_MULTIPLICATOR,
    GAME_OVER,
    START_GAME,
    NEW_GAME,
    MOUSE_MOVED,
    LEFT_CLICK,
    TEXTURES_LOADED,
    END_DISPLAY_LOADING
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