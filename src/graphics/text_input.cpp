#include "graphics/text_input.h"
#include "graphics/render.h"

#include <iostream>

tetrisTextInput::tetrisTextInput()
    : tetrisTextInput({ 0, 0 }, { 0, 0 })
{
}

tetrisTextInput::tetrisTextInput(Vector2 position, Size2 size)
    : _selected(false)
    , _text("")
    , _debug_hideTexture(false)
{
    _inputRectangleVector.reserve(3);
    const float sides_width = TEXT_SIDES.width * (size.height / TEXT_SIDES.height);
    _inputRectangleVector.push_back({ position.x, position.y, sides_width, size.height });
    _inputRectangleVector.push_back({ position.x + sides_width, position.y, size.width, size.height });
    _inputRectangleVector.push_back({ position.x + sides_width + size.width, position.y, sides_width, size.height });
    // Calculating the full width of the input;
    for (const auto& button : _inputRectangleVector) {
        _totalWidth += button.width;
    }
}

tetrisTextInput::~tetrisTextInput()
{

}

void tetrisTextInput::Update(Texture2D texture)
{
    const bool collide = checkCollisionPointRecArray(GetMousePosition(), &(_inputRectangleVector)[0], static_cast<int>(_inputRectangleVector.size()));
    auto textSize = static_cast<float>(MeasureText(_text.c_str(), FONT_SIZE_TEXT_INPUT));
    // Text Measurement Security check (-1 makes the DrawText unstable.)
    textSize = textSize == -1 ? 0 : textSize;
    
    // Selected is true if user is left clicking in the input text box. False otherwise
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        _selected = collide;
    if(IsKeyPressed(KEY_F3))
        _debug_hideTexture = !_debug_hideTexture;

    if(_selected)
    {
        const int key = GetKeyPressed();
        // Checking for a correct ascii key.
        // Checking if the contained text isn't bigger than the text input box.
        if ((key > KEY_NULL) && (key < KEY_ESCAPE) && (textSize < _inputRectangleVector[1].width))
            _text.push_back(key);
        // Checking for a backspace to erase the text. (Check before if text isn't empty.)
        else if(key == KEY_BACKSPACE && !_text.empty())
            _text.erase(_text.end()-1);
    }

    if(!_debug_hideTexture)
        DrawTextInput(texture);
    if(textSize > 0)
        DrawText(_text.c_str(), static_cast<int>(_inputRectangleVector[0].x + (_totalWidth - textSize) / 2), static_cast<int>(_inputRectangleVector[0].y + (_inputRectangleVector[0].height - FONT_SIZE_TEXT_INPUT) / 2), FONT_SIZE_TEXT_INPUT, {0,0,0,255});
}

void tetrisTextInput::DrawTextInput(Texture2D texture)
{
    DrawTexturePro(texture, { 0, 0, TEXT_SIDES.width, TEXT_SIDES.height }, _inputRectangleVector[0], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(texture, { TEXT_SIDES.width, 0, 1.0f, TEXT_HEIGHT}, _inputRectangleVector[1], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(texture, { TEXT_SIDES.width + 1.0f, 0, TEXT_SIDES.width, TEXT_SIDES.height }, _inputRectangleVector[2], { 0, 0 }, 0.0f, WHITE);
}

std::string tetrisTextInput::GetText() { return _text; }