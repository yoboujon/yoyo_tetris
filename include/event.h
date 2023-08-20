#ifndef HEADER_TETRIS_EVENT
#define HEADER_TETRIS_EVENT

#include <deque>
#include <map>
#include <any>

enum class eventType {
    MENU_OPENED,
    MENU_CLOSED,
    TETRIS_LINE_COMPLETED,
    SEND_SCORE,
    SEND_MULTIPLICATOR
};

enum class eventUser {
    ANY,
    TETRIS,
    UI,
    SCORE
};

class tetrisEvent {
public:
    tetrisEvent();
    ~tetrisEvent();
    // minimum being 1.
    template <class T>
    void callEvent(eventType type, eventUser forUser, T dataIn);
    void callEvent(eventType type, eventUser forUser);
    template <class T>
    bool OnEvent(eventType type, eventUser targetUser, T& dataOut);
    bool OnEvent(eventType type, eventUser targetUser);
    void ResetEvent(eventType type);

private:
    std::deque<eventType> _eventCalledDeque;
    std::deque<eventUser> _eventUsersDeque;
    std::map<eventType,std::deque<std::any>> _eventDataDeque;
};

#include "event.inl"

#endif // HEADER_TETRIS_EVENT