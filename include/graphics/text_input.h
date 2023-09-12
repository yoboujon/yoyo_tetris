#ifndef HEADER_TETRIS_TEXT_INPUT
#define HEADER_TETRIS_TEXT_INPUT

#include "lib.h"

#include "graphics/input_container.h"

class TetrisInputText : public TetrisInputContainer
{
public:
    TetrisInputText();
    TetrisInputText(Vector2 position, Size2 size);
    ~TetrisInputText();
    std::string GetText();
};

#endif // HEADER_TETRIS_TEXT_INPUT