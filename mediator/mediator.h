#pragma once
#include <string>

class BaseComponent;
class Mediator {
public:
    virtual void EventHandler(BaseComponent* sender, std::string event) const=0;
};

class BaseComponent {
public:
    BaseComponent(Mediator* mediator=NULL);
    void setMediator(Mediator* mediator);
protected:
    Mediator* _mediator;
};