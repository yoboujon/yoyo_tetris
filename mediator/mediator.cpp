#include "mediator.h"
#include <iostream>

BaseComponent::BaseComponent(Mediator* mediator)
    : _mediator(mediator)
{
}

void BaseComponent::setMediator(Mediator* mediator)
{
    this->_mediator = mediator;
}
