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
    CLOCKWISE,
    INVERTED,
    COUNTER_CLOCKWISE
};

class floatTetrisBlock {
public:
    floatTetrisBlock(tetromino::tetrominoNames name,Rectangle* gameRectangle, controlsTetris* gameControls);
    ~floatTetrisBlock();
    // Movement
    void Fall(const std::vector<Rectangle>& tetrisBlock);
    void Move(const std::vector<Rectangle>& tetrisBlock);
    void Rotate(const std::vector<Rectangle>& tetrisBlock);
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
    std::vector<Rectangle> constructReactangle(tetromino::tetrominoNames name, floatTetrisRotation rotation=floatTetrisRotation::NONE, bool calculateArea=true);
    int getRotationAngle(floatTetrisRotation rotation);
    
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
    void checkLine();
    const std::vector<Rectangle>& getRectangles();

private:
    std::vector<Rectangle> _tetrisBlocks;
    std::vector<Color> _tetrisColors;
    std::map<float, int> _lineMap;
};

#endif // HEADER_TETRIS_BLOCK