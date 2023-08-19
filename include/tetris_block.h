#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"

#include "controls.h"
#include "tetromino.h"

#include <array>
#include <stdint.h>
#include <vector>

constexpr int BASE_X = 250;
constexpr int BASE_Y = 40;
constexpr int FALL_SPEED = 7;
constexpr float KEY_TIMING = 0.05f;

class tetrisFloatBlock {
public:
    tetrisFloatBlock();
    tetrisFloatBlock(tetromino::tetrominoNames name,Rectangle* tetrisStage, tetrisControls* gameControls);
    ~tetrisFloatBlock();
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
    bool GameEnded(const std::vector<Rectangle>& tetrisBlock);
    void Display();
    void DisplayNext();

private:
    bool checkCollisionWith(const std::vector<Rectangle>& newRectangles, const std::vector<Rectangle>& collideRectangles);
    bool checkGameRectangle(const std::vector<Rectangle>& newRectangles);
    std::vector<Rectangle> moveX(int x);
    std::vector<Rectangle> moveY(int y);
    
    // Properties
    std::vector<Rectangle> _object;
    float _area_object;
    tetromino::tetrominoNames _name;
    Color _color;
    Vector2 _position;
    // State
    bool _placed;
    tetromino::tetrisRotation _rotation;
    // Game
    Rectangle* _tetrisStage;
    tetrisControls* _gameControls;
};

class tetrisStaticBlocks {
public:
    tetrisStaticBlocks();
    ~tetrisStaticBlocks();
    void Add(tetrisFloatBlock& tetrisBlock, Color tetrisColor);
    void Display();
    void checkLine();
    const std::vector<Rectangle>& getRectangles();

private:
    std::map<float, int> updateLineMap();
    std::vector<Rectangle> _tetrisBlocks;
    std::vector<Color> _tetrisColors;
    std::map<float, int> _lineMap;
};

#endif // HEADER_TETRIS_BLOCK