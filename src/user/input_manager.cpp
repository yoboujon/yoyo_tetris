#include "user/input_manager.h"

TetrisInputManager::TetrisInputManager()
{
    _keysEntered.reserve(256);
}

TetrisInputManager::~TetrisInputManager()
{
}

void TetrisInputManager::Update()
{
    auto keyPressed = GetKeyPressed();
    if (keyPressed != KEY_NULL)
    {
        _eventHandler->sendEvent(this, EventType::KEY_PRESSED);
        _keysEntered.push_back(keyPressed);
    }
}

std::vector<int> TetrisInputManager::GetKeys() { return _keysEntered; }
int TetrisInputManager::GetKey() { return (_keysEntered.empty() ? KEY_NULL : _keysEntered.back()); }
void TetrisInputManager::ClearKeys() { _keysEntered.clear(); }
