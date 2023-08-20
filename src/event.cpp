#include "event.h"
#include <iostream>

tetrisEvent::tetrisEvent()
{}

tetrisEvent::~tetrisEvent()
{}

void tetrisEvent::callEvent(eventType type, eventUser forUser) {
    eventCalledDeque.push_back(type);
    eventUsersDeque.push_back(forUser);
    //eventParameter.push_back(parameter);
}

bool tetrisEvent::OnEvent(eventType type, eventUser targetUser) {
    auto itCalled = std::find(eventCalledDeque.begin(), eventCalledDeque.end(), type);
    bool returnStatement(false);
    if (itCalled != eventCalledDeque.end()) {
        // if itCalled is found, userIndex for map cannot be null.
        // We get the iterator from the index of the itCalled.
        auto itUser = std::next(eventUsersDeque.begin(), (itCalled - eventCalledDeque.begin()));
        if(*itUser == targetUser)
        {
            // If correct user: Returning true and reseting the event.
            returnStatement = true;
            eventCalledDeque.erase(itCalled);
            eventUsersDeque.erase(itUser);
        }
    }
    return returnStatement;
}
