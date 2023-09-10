#include "graphics/button.h"
#include "user/mouse.h"

#include <iostream>

tetrisButton::tetrisButton()
    : tetrisButton({ 0, 0 }, { 0, 0 })
{
}

tetrisButton::tetrisButton(Vector2 position, Size2 size, textureStyle style, Size2 textureSize)
    : _buttonSize(size)
    , _textureSize(textureSize)
    , _style(style)
    , _state(buttonState::NONE)
    , _text("")
    , _textSize(0)
{
    if (style == textureStyle::CUSTOM_SHAPE) {
        _buttonRect.push_back({ position.x, position.y, size.width, size.height });
    } else {
        const float sides_width = TEXTURE_NORMAL_SIZE.width * (size.height / TEXTURE_NORMAL_SIZE.height);
        _buttonRect.push_back({ position.x, position.y, sides_width, size.height });
        _buttonRect.push_back({ position.x + sides_width, position.y, size.width, size.height });
        _buttonRect.push_back({ position.x + sides_width + size.width, position.y, sides_width, size.height });
    }
}

tetrisButton::~tetrisButton()
{
}

void tetrisButton::SetText(const std::string& txt, const int fontSize)
{
    _textSize = static_cast<float>(MeasureText(txt.c_str(), fontSize));
    _text = txt;
}

void tetrisButton::setPosition(Vector2 position)
{
    if (_style == textureStyle::CUSTOM_SHAPE) {
        _buttonRect[0].x = position.x;
        _buttonRect[0].y = position.y;
    } else {

        const float sides_width = TEXTURE_NORMAL_SIZE.width * (_buttonSize.height / TEXTURE_NORMAL_SIZE.height);
        _buttonRect.clear();
        _buttonRect.push_back({ position.x, position.y, sides_width, _buttonSize.height });
        _buttonRect.push_back({ position.x + sides_width, position.y, _buttonSize.width, _buttonSize.height });
        _buttonRect.push_back({ position.x + sides_width + _buttonSize.width, position.y, sides_width, _buttonSize.height });
    }
}

void tetrisButton::Update(Texture2D texture)
{
    // Checking State
    auto newState = (checkCollisionPointRecArray(GetMousePosition(), &(_buttonRect)[0], static_cast<int>(_buttonRect.size())) ? buttonState::HOVER : buttonState::NONE);
    
    // Changing mouse style
    if(newState != buttonState::NONE)
        TetrisMouse::GetInstance().SetMouse(MOUSE_CURSOR_POINTING_HAND);
    if((newState == buttonState::NONE) && (_state != buttonState::NONE))
        TetrisMouse::GetInstance().ResetMouse();
    
    // Interaction
    if (newState == buttonState::HOVER || newState == buttonState::PRESS) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            newState = buttonState::PRESS;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            newState = buttonState::RELEASE;
    }
    // Drawing Text
    DrawButton(texture);
    _state = newState;
}

void tetrisButton::DrawButton(Texture2D texture)
{
    // Selecting the correct texture offset with the given state
    const float textureSelector = _textureSize.height * static_cast<int>(_state);
    if (_style == textureStyle::CUSTOM_SHAPE) {
        DrawTexturePro(texture, { 0, textureSelector, _textureSize.width, _textureSize.height }, _buttonRect[0], { 0, 0 }, 0.0f, WHITE);
    } else {
        // Drawing the texture depending on the state
        DrawTexturePro(texture, { 0, textureSelector, TEXTURE_NORMAL_SIZE.width, TEXTURE_NORMAL_SIZE.height }, _buttonRect[0], { 0, 0 }, 0.0f, WHITE);
        DrawTexturePro(texture, { TEXTURE_NORMAL_SIZE.width, textureSelector, _textureSize.width, _textureSize.height }, _buttonRect[1], { 0, 0 }, 0.0f, WHITE);
        DrawTexturePro(texture, { TEXTURE_NORMAL_SIZE.width + _textureSize.width, textureSelector, TEXTURE_NORMAL_SIZE.width, TEXTURE_NORMAL_SIZE.height }, _buttonRect[2], { 0, 0 }, 0.0f, WHITE);
        // Drawing text
    }
    const Color actualColor = ((_state == buttonState::NONE || _state == buttonState::RELEASE) ? BLACK_TEXT : WHITE_TEXT);
    float totalWidth(0);
    for (const auto& button : _buttonRect) {
        totalWidth += button.width;
    }
    DrawText(_text.c_str(), static_cast<int>(_buttonRect[0].x + (totalWidth - _textSize) / 2), static_cast<int>(_buttonRect[0].y + (_buttonRect[0].height - FONT_SIZE) / 2), FONT_SIZE, actualColor);
}

bool tetrisButton::Clicked()
{
    return (_state == buttonState::RELEASE);
}

float tetrisButtongetTotalWidth(Size2 size)
{
    const float sides_width = TEXTURE_NORMAL_SIZE.width * (size.height / TEXTURE_NORMAL_SIZE.height);
    return ((sides_width * 2) + size.width);
}