#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"

#include "controls.h"
#include "raylib.h"
#include "tetromino.h"
#include <array>
#include <stdint.h>
#include <vector>

constexpr int BLOCK_SIZE = 20;
constexpr int BASE_X = 250;
constexpr int BASE_Y = 40;
constexpr double DEG_TO_RAD = PI/180;

enum class floatTetrisRotation {
    NONE,
    CLOCKWISE = 90,
    INVERTED = 180,
    COUNTER_CLOCKWISE = 270
};

class floatTetrisBlock {
public:
    floatTetrisBlock(tetromino::tetrominoNames name,Rectangle* gameRectangle, controlsTetris* gameControls);
    ~floatTetrisBlock();
    // Movement
    void Fall(const std::vector<Rectangle>& tetrisBlock);
    void Move(const std::vector<Rectangle>& tetrisBlock);
    void Rotate();
    // Getter
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
    void constructReactangle(tetromino::tetrominoNames name, floatTetrisRotation rotation=floatTetrisRotation::NONE, bool calculateArea=true);

    // Properties
    std::vector<Rectangle> _object;
    float _area_object;
    tetromino::tetrominoNames _name;
    Color _color;
    Vector2 _position;
    // State
    bool _placed;
    floatTetrisRotation _rotation;
    // Game
    Rectangle* _gameRectangle;
    controlsTetris* _gameControls;
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