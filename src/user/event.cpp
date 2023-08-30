#include "user/event.h"
#include <iostream>

tetrisEvent::tetrisEvent()
{}

tetrisEvent::~tetrisEvent()
{}

void tetrisEvent::callEvent(eventType type, eventUser forUser) {
    _eventCalledDeque.push_back(type);
    _eventUsersDeque.push_back(forUser);
}

bool tetrisEvent::OnEvent(eventType type, eventUser targetUser) {
    auto itCalled = std::find(_eventCalledDeque.begin(), _eventCalledDeque.end(), type);
    bool returnStatement(false);
    if (itCalled != _eventCalledDeque.end()) {
        // if itCalled is found, userIndex for map cannot be null.
        // We get the iterator from the index of the itCalled.
        auto itUser = std::next(_eventUsersDeque.begin(), (itCalled - _eventCalledDeque.begin()));
        if(*itUser == targetUser)
        {
            // If correct user: Returning true and reseting the event.
            returnStatement = true;
            _eventCalledDeque.erase(itCalled);
            _eventUsersDeque.erase(itUser);
        }
    }
    return returnStatement;
}
