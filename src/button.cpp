#include "button.h"

#include <iostream>

tetrisButton::tetrisButton()
    : tetrisButton(NULL, { 0, 0 }, { 0, 0 })
{
}

tetrisButton::tetrisButton(Texture2D* texture, Vector2 position, Size2 size, textureStyle style, Size2 textureSize)
    : _texture(texture)
    , _buttonSize(size)
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

void tetrisButton::Update()
{
    // Checking State
    _state = (checkCollisionPointRecArray(GetMousePosition(), &(_buttonRect)[0], static_cast<int>(_buttonRect.size())) ? buttonState::HOVER : buttonState::NONE);
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
    const float textureSelector = _textureSize.height * static_cast<int>(_state);
    if (_style == textureStyle::CUSTOM_SHAPE) {
        DrawTexturePro(*_texture, { 0, textureSelector, _textureSize.width, _textureSize.height }, _buttonRect[0], { 0, 0 }, 0.0f, WHITE);
    } else {
        // Drawing the texture depending on the state
        DrawTexturePro(*_texture, { 0, textureSelector, TEXTURE_NORMAL_SIZE.width, TEXTURE_NORMAL_SIZE.height }, _buttonRect[0], { 0, 0 }, 0.0f, WHITE);
        DrawTexturePro(*_texture, { TEXTURE_NORMAL_SIZE.width, textureSelector, _textureSize.width, _textureSize.height }, _buttonRect[1], { 0, 0 }, 0.0f, WHITE);
        DrawTexturePro(*_texture, { TEXTURE_NORMAL_SIZE.width + _textureSize.width, textureSelector, TEXTURE_NORMAL_SIZE.width, TEXTURE_NORMAL_SIZE.height }, _buttonRect[2], { 0, 0 }, 0.0f, WHITE);
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