#pragma once

#include "component.h"

class GameMediator : public Mediator
{
public:
    GameMediator(Component1* compPtr1, Component2* compPtr2);
    ~GameMediator();
    void EventHandler(BaseComponent *sender, std::string event) const override;
private:
    Component1 *_component1;
    Component2 *_component2;
};
