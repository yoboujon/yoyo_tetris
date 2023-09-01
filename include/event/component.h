#ifndef HEADER_TETRIS_COMPONENT
#define HEADER_TETRIS_COMPONENT

#include <any>

enum EventType {
    NONE = 0,
    MENU_OPENED,
    MENU_CLOSED,
    TETRIS_LINE_COMPLETED,
    SEND_SCORE,
    SEND_MULTIPLICATOR,
    START_GAME,
    CREATED_NEW_GAME
};

class BaseComponent;
class EventHandler
{
    public:
        virtual void sendEvent(BaseComponent* sender, EventType type) const=0;
        virtual void sendEvent(BaseComponent* sender, EventType type, const std::any& data) const=0;
};

class BaseComponent {
public:
    BaseComponent(EventHandler* event=nullptr);
    void setEventHandler(EventHandler* event);
protected:
    EventHandler* _eventHandler;
};

#endif // HEADER_TETRIS_COMPONENT