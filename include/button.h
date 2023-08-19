#ifndef HEADER_TETRIS_BUTTON
#define HEADER_TETRIS_BUTTON

#include "lib.h"
#include "raylib.h"

#include <string>
#include <vector>

constexpr float BUTTON_WIDTH = 128.0f;
constexpr float BUTTON_HEIGHT = 48.0f;
constexpr Color BLACK_TEXT = {44,53,77,255};
constexpr Color WHITE_TEXT = {223,224,232,255};
constexpr Size2 TEXTURE_SELECTOR = {16.0f,16.0f};
constexpr Size2 TEXTURE_NORMAL_SIZE = {2.0f,16.0f};
constexpr int FONT_SIZE = 20;

enum class textureStyle {
    NORMAL,
    CUSTOM_SHAPE
};

enum class buttonState {
    NONE,
    HOVER,
    PRESS,
    RELEASE
};

class tetrisButton {
    public:
        tetrisButton();
        tetrisButton(Texture2D* texture, Vector2 position, Size2 size);
        ~tetrisButton();
        void SetText(const std::string& txt);
        void Update();
        bool Clicked();
    private:
        void DrawButton();
        std::array<Rectangle,3> _buttonRect;
        Texture2D* _texture;
        Size2 _buttonSize;
        buttonState _state;
        std::string _text;
        int _textSize;
};

float tetrisButtongetTotalWidth(Size2 size);

#endif //HEADER_TETRIS_BUTTON