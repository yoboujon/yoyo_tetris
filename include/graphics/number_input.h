#ifndef HEADER_TETRIS_NUM_INPUT
#define HEADER_TETRIS_NUM_INPUT

#include "lib.h"

#include "graphics/input_container.h"

class TetrisInputNumber : public TetrisInputContainer
{
public:
    TetrisInputNumber();
    TetrisInputNumber(Vector2 position, Size2 size);
    ~TetrisInputNumber();
    int GetInt();
};

#endif // HEADER_TETRIS_NUM_INPUT