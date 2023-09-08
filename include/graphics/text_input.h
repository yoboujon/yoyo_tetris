#ifndef HEADER_TETRIS_TEXT_INPUT
#define HEADER_TETRIS_TEXT_INPUT

#include "lib.h"

#include "user/controls.h"

#include <string>
#include <vector>

constexpr Size2 TEXT_SIDES = {2.0f,16.0f};
constexpr float TEXT_HEIGHT = 16.0f;
constexpr float TEXT_WIDTH = 2*(TEXT_SIDES.width) + 1;
constexpr float FONT_SIZE_TEXT_INPUT = 10.0f;

class tetrisTextInput {
public:
    tetrisTextInput();
    tetrisTextInput(Vector2 position, Size2 size);
    ~tetrisTextInput();
    void Update(Texture2D texture);
    std::string GetText();
private:
    void DrawTextInput(Texture2D texture);
    std::vector<Rectangle> _inputRectangleVector;
    float _totalWidth;
    bool _selected;
    std::string _text;
    bool _debug_hideTexture;
};

#endif // HEADER_TETRIS_TEXT_INPUT