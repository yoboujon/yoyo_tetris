#include "controls.h"

#include <iostream>

tetrisControls::tetrisControls(float* elapsedPtr)
    : _elapsedPtr(elapsedPtr)
    , _timingCommand(0.0f)
    , _keyPressed(keyState::UNSET)
{}

tetrisControls::~tetrisControls()
{}

bool tetrisControls::TempoKey(bool keyDown, float timing)
{
    // Init
    if( ((_keyPressed == keyState::UNSET) || (_keyPressed == keyState::NOT_PRESSED)) && keyDown ) 
    {
        _keyPressed = keyState::PRESSED;
        _timingCommand = *_elapsedPtr + timing;
        return (_keyPressed == keyState::UNSET);
    }

    // Actual check
    if( keyDown && (*_elapsedPtr >= _timingCommand) )
    {
        _keyPressed = keyState::NOT_PRESSED;
        return true;
    }
    return false;
}

bool tetrisControls::IsKeyDownTiming(int key, float timing)
{
    return TempoKey(::IsKeyDown(key), timing);
}

bool tetrisControls::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}