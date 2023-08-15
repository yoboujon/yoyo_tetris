#include "controls.h"
#include <iostream>

controlsTetris::controlsTetris()
    : _elapsed(0.0f)
{}

controlsTetris::~controlsTetris()
{}

// ! BUFFER Working, but way too slow to actually be useful...
// ! Maybe using a vector would be faster?
bool controlsTetris::IsKeyDownTiming(int key, float timing)
{
    _elapsed += GetFrameTime();
    auto itKey = _keyPressed.find(key);
    auto itTiming = _timingCommand.find(key);
    
    // Init
    if( ((itKey == _keyPressed.end()) ||  (itKey->second == false)) && IsKeyDown(key)) 
    {
        _keyPressed[key] = true;
        _timingCommand[key] = _elapsed + timing;
        return (itKey == _keyPressed.end());;
    }

    // Actual check
    if( IsKeyDown(key) && (_elapsed >= itTiming->second) )
    {
        _keyPressed[key] = false;
        return true;
    }
    return false;
}

bool controlsTetris::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}