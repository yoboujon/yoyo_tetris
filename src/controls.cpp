#include "controls.h"
#include "raylib.h"
#include <iostream>

controlsTetris::controlsTetris()
    : _elapsed(0.0f)
    , _timingCommand(0.0f)
    , _keyPressed(keyState::UNSET)
{}

controlsTetris::~controlsTetris()
{}

bool controlsTetris::TempoKey(bool keyDown, float timing)
{
    _elapsed += GetFrameTime();
    
    // Init
    if( ((_keyPressed == keyState::UNSET) || (_keyPressed == keyState::NOT_PRESSED)) && keyDown ) 
    {
        _keyPressed = keyState::PRESSED;
        _timingCommand = _elapsed + timing;
        return (_keyPressed == keyState::UNSET);
    }

    // Actual check
    if( keyDown && (_elapsed >= _timingCommand) )
    {
        _keyPressed = keyState::NOT_PRESSED;
        return true;
    }
    return false;
}

bool controlsTetris::IsKeyDownTiming(int key, float timing)
{
    return TempoKey(::IsKeyDown(key), timing);
}

bool controlsTetris::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}