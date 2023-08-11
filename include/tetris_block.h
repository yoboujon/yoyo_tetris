#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"
#include "raylib.h"

#include <vector>

#define BLOCK_SIZE 20
#define BASE_X 254
#define BASE_Y 44

class tetrisBlock {
    public:
        tetrisBlock();
        ~tetrisBlock();
        void Move();
    private:
        bool canMove();
        std::vector<Rectangle> _object;
        Vector2 _position;
        int _height;
};

#endif // HEADER_TETRIS_BLOCK