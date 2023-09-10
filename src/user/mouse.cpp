#include "user/mouse.h"

TetrisMouse::TetrisMouse()
    : _mouseStatus(MOUSE_CURSOR_DEFAULT)
    , _mouseChanged(false)
    , _specialCursor(false)
{
}

TetrisMouse::~TetrisMouse()
{}

void TetrisMouse::SetMouse(MouseCursor cursor)
{
    _mouseStatus = cursor;
    _mouseChanged = true;
    _specialCursor = true;
}

void TetrisMouse::UpdateMouse()
{
    // If the mouse cursor changed set cursor to the given status
    if(_mouseChanged)
    {
        _mouseChanged = false;
        SetMouseCursor(_mouseStatus);
    }
}

void TetrisMouse::ResetMouse()
{
    _mouseChanged = false;
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}