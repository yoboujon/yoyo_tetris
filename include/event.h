#ifndef HEADER_TETRIS_EVENT
#define HEADER_TETRIS_EVENT

#include <deque>
#include <map>

enum class eventType {
    MENU_OPENED,
    MENU_CLOSED
};

enum class eventUser {
    ANY,
    TETRIS,
    UI
};

class tetrisEvent {
public:
    tetrisEvent();
    ~tetrisEvent();
    // minimum being 1.
    void callEvent(eventType type, eventUser forUser);
    bool OnEvent(eventType type, eventUser user);
    void ResetEvent(eventType type);

private:
    std::deque<eventType> eventCalledDeque;
    std::deque<eventUser> eventUsersDeque;
    //std::deque<void*> eventParameter;
};

#endif // HEADER_TETRIS_EVENT