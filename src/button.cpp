#include "button.h"
#include "lib.h"
#include "raylib.h"
#include <iostream>
#include <stdint.h>

tetrisButton::tetrisButton()
    : tetrisButton(NULL, { 0, 0 }, { 0, 0 })
{
}

tetrisButton::tetrisButton(Texture2D* texture, Vector2 position, Size2 size)
    : _texture(texture)
    , _buttonSize(size)
    , _state(buttonState::NONE)
    , _text("")
    , _textSize(0)
{
    const float sides_width = TEXTURE_NORMAL_SIZE.width * (size.height/TEXTURE_NORMAL_SIZE.height);
    _buttonRect[0] = { position.x, position.y, sides_width, size.height };
    _buttonRect[1] = { position.x + sides_width, position.y, _buttonSize.width, size.height };
    _buttonRect[2] = { position.x + sides_width + _buttonSize.width, position.y, sides_width, size.height };
}

tetrisButton::~tetrisButton()
{
}

void tetrisButton::SetText(const std::string& txt)
{
    _textSize = MeasureText(txt.c_str(), FONT_SIZE);
    _text = txt;
}

void tetrisButton::Update()
{
    // Checking State
    _state = (checkCollisionPointRecArray(GetMousePosition(), &(_buttonRect)[0], _buttonRect.size()) ? buttonState::HOVER : buttonState::NONE);
    // Interaction
    if (_state == buttonState::HOVER || _state == buttonState::PRESS) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            _state = buttonState::PRESS;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            _state = buttonState::RELEASE;
    }
    // Drawing Text
    DrawButton();
}

void tetrisButton::DrawButton()
{
    // Selecting the correct texture offset with the given state
    const float textureSelector = TEXTURE_SELECTOR.height * static_cast<int>(_state);
    // Drawing the texture depending on the state
    DrawTexturePro(*_texture, { 0, textureSelector, TEXTURE_NORMAL_SIZE.width, TEXTURE_NORMAL_SIZE.height }, _buttonRect[0], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(*_texture, { TEXTURE_NORMAL_SIZE.width, textureSelector, TEXTURE_SELECTOR.width, TEXTURE_SELECTOR.height }, _buttonRect[1], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(*_texture, { TEXTURE_NORMAL_SIZE.width + TEXTURE_SELECTOR.width, textureSelector, TEXTURE_NORMAL_SIZE.width, TEXTURE_NORMAL_SIZE.height }, _buttonRect[2], { 0, 0 }, 0.0f, WHITE);
    // Drawing text
    const Color actualColor = ((_state == buttonState::NONE || _state == buttonState::RELEASE) ? BLACK_TEXT : WHITE_TEXT);
    float totalWidth(0);
    for(const auto& button : _buttonRect)
    {
        totalWidth += button.width;
    }
    DrawText(_text.c_str(), _buttonRect[0].x + (totalWidth - _textSize) / 2, _buttonRect[0].y + (_buttonRect[0].height - FONT_SIZE) / 2, FONT_SIZE, actualColor);
}

bool tetrisButton::Clicked()
{
    return (_state == buttonState::RELEASE);
}

float tetrisButtongetTotalWidth(Size2 size)
{
    const float sides_width = TEXTURE_NORMAL_SIZE.width * (size.height/TEXTURE_NORMAL_SIZE.height);
    return ((sides_width*2) + size.width);
}