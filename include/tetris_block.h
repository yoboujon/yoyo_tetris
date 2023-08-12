#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"
#include "raylib.h"

#include <vector>

#define BLOCK_SIZE 20
#define BASE_X 254
#define BASE_Y 44

class floatTetrisBlock {
public:
    floatTetrisBlock(Rectangle* screen);
    ~floatTetrisBlock();
    void Move(const std::vector<Rectangle>& tetrisBlock);
    bool Placed();
    Rectangle* getRectangle(int index);
    const std::vector<Rectangle>& getRectangles();

private:
    void printRec();
    bool canMove();
    bool collideWith(const std::vector<Rectangle>& tetrisBlock);
    Rectangle* _screen;
    bool _placed;
    std::vector<Rectangle> _object;
    Vector2 _position;
    int _height;
};

class staticTetrisBlocks {
public:
    staticTetrisBlocks();
    ~staticTetrisBlocks();
    void Add(floatTetrisBlock& tetrisBlock);
    void Display();
    const std::vector<Rectangle>& getRectangles();

private:
    std::vector<Rectangle> _tetrisblocks;
};

#endif // HEADER_TETRIS_BLOCK