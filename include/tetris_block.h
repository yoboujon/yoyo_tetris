#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"

#include "controls.h"
#include "tetromino.h"
#include <array>
#include <stdint.h>
#include <vector>

constexpr int BLOCK_SIZE = 20;
constexpr int BASE_X = 250;
constexpr int BASE_Y = 40;

class floatTetrisBlock {
public:
    floatTetrisBlock(tetromino::tetrominoNames name,Rectangle* gameRectangle, controlsTetris* gameControls);
    ~floatTetrisBlock();
    void Fall(const std::vector<Rectangle>& tetrisBlock);
    void Move(const std::vector<Rectangle>& tetrisBlock);
    bool Placed();
    Rectangle* getRectangle(int index);
    const std::vector<Rectangle>& getRectangles();
    Color getColor();
    tetromino::tetrominoNames getName();

private:
    void printRec();
    bool checkCollisionWith(const std::vector<Rectangle>& newRectangles, const std::vector<Rectangle>& collideRectangles);
    bool checkGameRectangle(const std::vector<Rectangle>& newRectangles);
    std::vector<Rectangle> moveX(int x);
    std::vector<Rectangle> moveY(int y);
    tetromino::tetrominoNames _name;
    bool _placed;
    std::vector<Rectangle> _object;
    float _area_object;
    Rectangle* _gameRectangle;
    controlsTetris* _gameControls;
    Color _color;
};

class staticTetrisBlocks {
public:
    staticTetrisBlocks();
    ~staticTetrisBlocks();
    void Add(floatTetrisBlock& tetrisBlock, Color tetrisColor);
    void Display();
    const std::vector<Rectangle>& getRectangles();

private:
    std::vector<Rectangle> _tetrisBlocks;
    std::vector<Color> _tetrisColors;
};

#endif // HEADER_TETRIS_BLOCK