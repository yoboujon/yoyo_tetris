#include "event/component.h"

BaseComponent::BaseComponent(EventHandler* event)
    : _eventHandler(event)
{
}

void BaseComponent::setEventHandler(EventHandler* event)
{
    // Setting the eventhandler of the component
    _eventHandler = event;
}