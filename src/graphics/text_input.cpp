#include "graphics/text_input.h"
#include "graphics/render.h"
#include "user/input_manager.h"
#include "user/mouse.h"

#include <iostream>

tetrisTextInput::tetrisTextInput()
    : tetrisTextInput({ 0, 0 }, { 0, 0 })
{
}

tetrisTextInput::tetrisTextInput(Vector2 position, Size2 size)
    : _totalWidth(0)
    , _selected(false)
    , _text("")
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
    const float textSize = static_cast<float>(MeasureText(_text.c_str(), FONT_SIZE_TEXT_INPUT));
    const bool collide = checkCollisionPointRecArray(GetMousePosition(), &(_inputRectangleVector)[0], static_cast<int>(_inputRectangleVector.size()));
    bool selected = _selected;
    float offset(0);
    auto& tetrisInput = TetrisInputManager::GetInstance();

    // Changing mouse style
    if (collide)
        TetrisMouse::GetInstance().SetMouse(MOUSE_CURSOR_IBEAM);
    if (!collide && _collide)
        TetrisMouse::GetInstance().ResetMouse();

    // Selected is true if user is left clicking in the input text box. False otherwise
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        selected = collide;
    // If entered selection mode, clear the TetrisInputManager Key Buffer.
    if (selected && !_selected)
        tetrisInput.ClearKeys();

    if (selected) {
        // Graphical
        offset = 1 * TEXTURE_OFFSET;

        // Logical
        // Logical
        auto keys = tetrisInput.GetKeys();
        if (!keys.empty()) {
            for (auto key : keys) {
                // Checking for a correct ascii key.
                // Checking if the contained text isn't bigger than the text input box.
                if (((key > KEY_NULL) && (key < KEY_ESCAPE)) && (textSize < _inputRectangleVector[1].width))
                    _text.push_back(key); // Add the ASCII character to _text.
                // Checking for a backspace to erase the text. (Check before if text isn't empty.)
                else if (key == KEY_BACKSPACE && !_text.empty())
                    _text.erase(_text.end() - 1);
            }
            tetrisInput.ClearKeys();
        }
    }

    if (!_text.empty() && !_selected) {
        offset = 2 * TEXTURE_OFFSET;
        // std::cout << _text << std::endl;
    }

    DrawTextInput(texture, offset);
    if (textSize > 0)
        DrawText(_text.c_str(), static_cast<int>(_inputRectangleVector[0].x + (_totalWidth - textSize) / 2), static_cast<int>(_inputRectangleVector[0].y + (_inputRectangleVector[0].height - FONT_SIZE_TEXT_INPUT) / 2), FONT_SIZE_TEXT_INPUT, { 0, 0, 0, 255 });
    _collide = collide;
    _selected = selected;
}

void tetrisTextInput::DrawTextInput(Texture2D texture, float offset)
{
    DrawTexturePro(texture, { offset, 0, TEXT_SIDES.width, TEXT_SIDES.height }, _inputRectangleVector[0], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(texture, { offset + TEXT_SIDES.width, 0, 1.0f, TEXT_HEIGHT }, _inputRectangleVector[1], { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(texture, { offset + TEXT_SIDES.width + 1.0f, 0, TEXT_SIDES.width, TEXT_SIDES.height }, _inputRectangleVector[2], { 0, 0 }, 0.0f, WHITE);
}

std::string tetrisTextInput::GetText() { return _text; }