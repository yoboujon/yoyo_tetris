template <class T>
void tetrisEvent::callEvent(eventType type, eventUser forUser, T dataIn)
{
    callEvent(type,forUser);
    _eventDataDeque[type].push_back(dataIn);
}

template <class T>
bool tetrisEvent::OnEvent(eventType type, eventUser targetUser, T& dataOut)
{
    auto itCalled = std::find(_eventCalledDeque.begin(), _eventCalledDeque.end(), type);
    auto itDataMap = _eventDataDeque.find(type); 
    bool returnStatement(false);
    if ((itCalled != _eventCalledDeque.end()) && (itDataMap != _eventDataDeque.end())) {
        // if itCalled is found, userIndex for map cannot be null.
        // We get the iterator from the index of the itCalled.
        auto itUser = std::next(_eventUsersDeque.begin(), (itCalled - _eventCalledDeque.begin()));
        auto itData = itDataMap->second.end()-1;
        if(*itUser == targetUser)
        {
            // If correct user: Returning true and reseting the event.
            returnStatement = true;
            _eventCalledDeque.erase(itCalled);
            _eventUsersDeque.erase(itUser);
            dataOut = std::any_cast<T>(*itData);
            itDataMap->second.erase(itData);
        }
    }
    return returnStatement;
}
