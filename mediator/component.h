#pragma once

#include "mediator.h"

class Component1 : public BaseComponent {
public:
    Component1(int a);
    ~Component1();
    void DoA();
private:
    int _a;
};

class Component2 : public BaseComponent {
public:
    Component2(int b);
    ~Component2();
    void DoB();
    void DoC(std::string str);
private:
    int _b;
};