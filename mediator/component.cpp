#include "component.h"
#include <iostream>

Component1::Component1(int a)
    : _a(a)
{
}

Component1::~Component1()
{

}

void Component1::DoA()
{
    this->_mediator->EventHandler(this,"Component CALL :"+std::to_string(_a));
}

Component2::Component2(int b)
    :_b(b)
{
}

Component2::~Component2()
{

}

void Component2::DoB()
{
    this->_mediator->EventHandler(this,"Component CALL :"+std::to_string(_b));
}

void Component2::DoC(std::string str)
{
    std::cout << "[Component2] : Wow i received : " << str << std::endl;
}