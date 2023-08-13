#include "controls.h"

controlsTetris::controlsTetris()
{}

controlsTetris::~controlsTetris()
{}

bool controlsTetris::IsKeyDownTiming(int key, float timing)
{
    _elapsed += GetFrameTime();
    if((_timingCommand == 0 && IsKeyDown(key)) || (IsKeyDown(key) && (_elapsed >= _timingCommand)))
    {
        _timingCommand = _elapsed + timing;
        return true;
    }
    return false;
}

bool controlsTetris::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}