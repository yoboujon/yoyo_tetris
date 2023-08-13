#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"

#include "controls.h"
#include <stdint.h>
#include <vector>

#define BLOCK_SIZE 20
#define BASE_X 250
#define BASE_Y 40

class floatTetrisBlock {
public:
    floatTetrisBlock(Rectangle* gameRectangle, controlsTetris* gameControls);
    ~floatTetrisBlock();
    void Fall(const std::vector<Rectangle>& tetrisBlock);
    void Move(const std::vector<Rectangle>& tetrisBlock);
    bool Placed();
    Rectangle* getRectangle(int index);
    const std::vector<Rectangle>& getRectangles();

private:
    void printRec();
    bool checkCollisionWith(const std::vector<Rectangle>& newRectangles,const std::vector<Rectangle>& collideRectangles);
    bool checkGameRectangle(const std::vector<Rectangle>& newRectangles);
    std::vector<Rectangle> moveX(int x);
    std::vector<Rectangle> moveY(int y);
    bool _placed;
    std::vector<Rectangle> _object;
    float _area_object;
    Rectangle* _gameRectangle;
    controlsTetris* _gameControls;
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