#include "button.h"
#include "lib.h"
#include "raylib.h"
#include <iostream>
#include <stdint.h>

tetrisButton::tetrisButton()
    : tetrisButton(NULL, { 0, 0 }, ButtonStyle::NONE)
{
}

tetrisButton::tetrisButton(Texture2D* texture, Vector2 position, ButtonStyle style)
    : _texture(texture)
    , _state(buttonState::NONE)
    , _text("")
    , _textSize(0)
{
    if (style != ButtonStyle::NONE) {
        _button = { (SCREEN_WIDTH - BUTTON_WIDTH + position.x) / 2, (SCREEN_HEIGHT / 2) - (position.y), BUTTON_WIDTH, BUTTON_HEIGHT };
    } else {
        _button = { position.x, position.y, BUTTON_WIDTH, BUTTON_HEIGHT };
    }
}

tetrisButton::~tetrisButton()
{
}

void tetrisButton::SetText(const std::string& txt) {
    _textSize = MeasureText(txt.c_str(), FONT_SIZE);
    _text = txt;
}

void tetrisButton::Update()
{
    // Checking State
    _state = (CheckCollisionPointRec(GetMousePosition(), _button) ? buttonState::HOVER : buttonState::NONE);
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
    Rectangle textureSelector = { 0, TEXTURE_SELECTOR.height * static_cast<int>(_state), TEXTURE_SELECTOR.width, TEXTURE_SELECTOR.height };
    // Drawing the texture depending on the state
    DrawTexturePro(*_texture, textureSelector, _button, {0,0}, 0.0f, WHITE);
    // Drawing text
    Color actualColor = ((_state == buttonState::NONE || _state == buttonState::RELEASE) ? BLACK_TEXT : WHITE_TEXT);
    DrawText(_text.c_str(), _button.x + (_button.width - _textSize) / 2, _button.y + (_button.height - FONT_SIZE) / 2, FONT_SIZE, actualColor);
}

bool tetrisButton::Clicked()
{
    return (_state == buttonState::RELEASE);
}