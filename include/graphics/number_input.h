#ifndef HEADER_TETRIS_NUM_INPUT
#define HEADER_TETRIS_NUM_INPUT

#include "lib.h"

#include "graphics/input_container.h"
#include "raylib.h"

constexpr Size2 UP_DOWN_TEXTURE = { 11.0f,8.0f };

enum class upDownState {
    NONE,
    HOVER,
    PRESS,
    RELEASE
};

class TetrisInputNumber : public TetrisInputContainer
{
public:
    TetrisInputNumber();
    TetrisInputNumber(Vector2 position, Size2 size);
    ~TetrisInputNumber();
    void Update(const std::vector<Texture2D>& texture);
    int GetInt();
private:
    std::vector<Rectangle> _upDownRect;
    std::array<upDownState, 2> _states;
};

#endif // HEADER_TETRIS_NUM_INPUT