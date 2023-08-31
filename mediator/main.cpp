#include <iostream>
#include "mediator.h"
#include "component.h"
#include "gamemediator.h"

int main(void)
{
    Component1 coolComponent(1);
    Component2 betterComponent(2);
    GameMediator game(&coolComponent,&betterComponent);

    coolComponent.DoA();
    betterComponent.DoB();
    return 0;
}