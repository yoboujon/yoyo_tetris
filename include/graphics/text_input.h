#ifndef HEADER_TETRIS_TEXT_INPUT
#define HEADER_TETRIS_TEXT_INPUT

#include "lib.h"
#include "event/component.h"

#include "user/controls.h"

#include <string>
#include <vector>

constexpr Size2 TEXT_SIDES = {2.0f,16.0f};
constexpr float TEXT_HEIGHT = 16.0f;
constexpr float TEXT_WIDTH = 2*(TEXT_SIDES.width) + 1;
constexpr float FONT_SIZE_TEXT_INPUT = 20.0f;
constexpr float TEXTURE_OFFSET = 5.0f;

class tetrisTextInput
{
public:
    tetrisTextInput();
    tetrisTextInput(Vector2 position, Size2 size);
    ~tetrisTextInput();
    void Update(Texture2D texture);
    std::string GetText();
private:
    void DrawTextInput(Texture2D texture, float offset);
    std::vector<Rectangle> _inputRectangleVector;
    float _totalWidth;
    bool _selected;
    bool _collide;
    std::string _text;
};

#endif // HEADER_TETRIS_TEXT_INPUT