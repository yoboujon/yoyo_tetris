#include "gamemediator.h"
#include <iostream>

GameMediator::GameMediator(Component1* compPtr1, Component2* compPtr2)
    : _component1(compPtr1)
    , _component2(compPtr2)
{
    _component1->setMediator(this);
    _component2->setMediator(this);
}

GameMediator::~GameMediator()
{
}

void GameMediator::EventHandler(BaseComponent *sender, std::string event) const
{
    std::cout << "[GameMediator] Event received: " << event << std::endl;
    if(event == "Component CALL :1")
        _component2->DoC(event);
}