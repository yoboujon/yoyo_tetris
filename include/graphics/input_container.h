#ifndef HEADER_TETRIS_INPUT_CONTAINER
#define HEADER_TETRIS_INPUT_CONTAINER

#include "lib.h"
#include "raylib.h"

#include <string>
#include <vector>
#include <functional>

constexpr Size2 TEXT_SIDES = {2.0f,16.0f};
constexpr float TEXT_HEIGHT = 16.0f;
constexpr float TEXT_WIDTH = 2*(TEXT_SIDES.width) + 1;
constexpr float BASE_TEXTURE_OFFSET = 5.0f;
constexpr int FONT_SIZE_TEXT_INPUT = 20;

class TetrisInputContainer {
    public:
        TetrisInputContainer(Vector2 position, Size2 size, std::function<bool(int)> keyCondition);
        ~TetrisInputContainer();
        virtual void Update(const std::vector<Texture2D>& texture);
    protected:
        //void Update(Texture2D texture);
        virtual void DrawTextInput(Texture2D texture, float offset);
        std::vector<Rectangle> _inputRectangleVector;
        float _totalWidth;
        bool _selected;
        bool _collide;
        std::string _text;
    private:
        std::function<bool(int)> _keyCondition;
};

#endif //HEADER_TETRIS_INPUT_CONTAINER