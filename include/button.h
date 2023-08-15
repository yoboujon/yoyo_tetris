#ifndef HEADER_TETRIS_BUTTON
#define HEADER_TETRIS_BUTTON

#include "lib.h"
#include "raylib.h"
#include "raymath.h"
#include <string>

constexpr float BUTTON_WIDTH = 128.0f;
constexpr float BUTTON_HEIGHT = 48.0f;
constexpr Rectangle TEXTURE_SELECTOR = { 0,0,32.0f, 12.0f };
constexpr int FONT_SIZE = 20;

enum class ButtonStyle {
    NONE,
    CENTERED,
};

enum class buttonState {
    NONE,
    HOVER,
    START_PRESS,
    FINISH_PRESS
};

class tetrisButton {
    public:
        tetrisButton();
        tetrisButton(Texture2D* texture, Vector2 position, ButtonStyle style=ButtonStyle::NONE);
        ~tetrisButton();
        void SetText(const std::string& txt);
        void Update();
        bool Clicked();
    private:
        Rectangle _button;
        Texture2D* _texture;
        buttonState _state;
        std::string _text;
};

#endif //HEADER_TETRIS_BUTTON