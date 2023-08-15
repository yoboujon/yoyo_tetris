#include "button.h"
#include "lib.h"
#include "raylib.h"
#include <iostream>
#include <stdint.h>

tetrisButton::tetrisButton(Vector2* mousePtr, Texture2D* texture, Vector2 position, ButtonStyle style)
    : _mouse(mousePtr)
    , _texture(texture)
    , _state(buttonState::NONE)
    , _text("")
{
    if(style != ButtonStyle::NONE)
    {
        _button = {(SCREEN_WIDTH-BUTTON_WIDTH+position.x)/2, (SCREEN_HEIGHT/2)-(position.y), BUTTON_WIDTH, BUTTON_HEIGHT};
    }
    else {
        _button = {position.x, position.y, BUTTON_WIDTH, BUTTON_HEIGHT};
    }
}

tetrisButton::~tetrisButton()
{

}

void tetrisButton::SetText(const std::string& txt)
{
    _text = txt;
}

void tetrisButton::Update()
{
    // Drawing
    // Updating state with the mouse
    _state = (CheckCollisionPointRec(*_mouse, _button) ? buttonState::HOVER : buttonState::NONE);
    // Selecting the correct texture offset with the given state
    Rectangle textureSelector = { 0, TEXTURE_SELECTOR.height * static_cast<int>(_state), TEXTURE_SELECTOR.width, TEXTURE_SELECTOR.height};
    // Drawing the texture depending on the state
    DrawTexturePro(*_texture, textureSelector, _button, NULL_VECTOR2, 0.0f, WHITE);
    // Drawing text
    int textSize = MeasureText(_text.c_str(), FONT_SIZE);
    DrawText(_text.c_str(), _button.x + (_button.width - textSize) / 2, _button.y + (_button.height - FONT_SIZE) / 2, FONT_SIZE, RAYWHITE);

    // Interaction
    if(_state == buttonState::HOVER || _state == buttonState::START_PRESS)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) _state = buttonState::START_PRESS;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) _state = buttonState::FINISH_PRESS;
    }
}

bool tetrisButton::Clicked()
{
    return (_state == buttonState::FINISH_PRESS); 
}