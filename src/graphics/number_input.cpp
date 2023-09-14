#include "graphics/number_input.h"
#include "graphics/input_container.h"
#include "user/mouse.h"

#include <iostream>
#include <string>

TetrisInputNumber::TetrisInputNumber()
    : TetrisInputContainer({ 0 }, { 0 }, [](int key) { return false; })
{
}

TetrisInputNumber::TetrisInputNumber(Vector2 position, Size2 size)
    : TetrisInputContainer(position, size, [](int key) { return (key >= KEY_ZERO && key <= KEY_NINE) || (key >= KEY_KP_0 && key <= KEY_KP_9); })
    , _states({upDownState::NONE, upDownState::NONE})
{
    _upDownRect.reserve(2);
    float ratioWidth = (size.height / 2) * (UP_DOWN_TEXTURE.width / UP_DOWN_TEXTURE.height);
    _upDownRect.push_back({ position.x + _totalWidth, position.y, ratioWidth, (size.height / 2) });
    _upDownRect.push_back({ position.x + _totalWidth, position.y + (size.height/2), ratioWidth, (size.height / 2) });
    _text = "0";
}

TetrisInputNumber::~TetrisInputNumber()
{
}

void TetrisInputNumber::Update(const std::vector<Texture2D>& texture)
{
    TetrisInputContainer::Update(texture);

    // Checking State
    auto newStateUp = (checkCollisionPointRecArray(GetMousePosition(), &(_upDownRect)[0], 1) ? upDownState::HOVER : upDownState::NONE);
    auto newStateDown = (checkCollisionPointRecArray(GetMousePosition(), &(_upDownRect)[1], 1) ? upDownState::HOVER : upDownState::NONE);

    // Changing mouse style
    if(newStateUp != upDownState::NONE)
        TetrisMouse::GetInstance().SetMouse(MOUSE_CURSOR_POINTING_HAND);
    if((newStateUp == upDownState::NONE) && (_states[0] != upDownState::NONE))
        TetrisMouse::GetInstance().ResetMouse();

    if(newStateDown != upDownState::NONE)
        TetrisMouse::GetInstance().SetMouse(MOUSE_CURSOR_POINTING_HAND);
    if((newStateDown == upDownState::NONE) && (_states[1] != upDownState::NONE))
        TetrisMouse::GetInstance().ResetMouse();

    // Interaction
    if (newStateUp == upDownState::HOVER || newStateUp == upDownState::PRESS) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            newStateUp = upDownState::PRESS;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            newStateUp = upDownState::RELEASE;
    }
    if (newStateDown == upDownState::HOVER || newStateDown == upDownState::PRESS) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            newStateDown = upDownState::PRESS;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            newStateDown = upDownState::RELEASE;
    }

    // Drawing Text
    _states = {newStateUp,newStateDown};

    if(_states[0] == upDownState::RELEASE)
        _text = std::to_string(std::stoi(_text)+1);
    if(_states[1] == upDownState::RELEASE)
        _text = std::to_string(std::stoi(_text)-1);

    const float textureSelectorUp = UP_DOWN_TEXTURE.height * static_cast<int>(_states[0]);
    const float textureSelectorDown = UP_DOWN_TEXTURE.height * static_cast<int>(_states[1]);
    DrawTexturePro(texture.at(1), { 0, textureSelectorUp, UP_DOWN_TEXTURE.width, UP_DOWN_TEXTURE.height }, _upDownRect[0], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(texture.at(1), { UP_DOWN_TEXTURE.width, textureSelectorDown, UP_DOWN_TEXTURE.width, UP_DOWN_TEXTURE.height }, _upDownRect[1], { 0, 0 }, 0.0f, WHITE);
}

int TetrisInputNumber::GetInt() { return std::stoi(_text); }