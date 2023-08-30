#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"

#include "user/controls.h"
#include "game/tetromino.h"
#include "graphics/ui.h"

#include <array>
#include <stdint.h>
#include <vector>

constexpr int BASE_X = 250;
constexpr int BASE_Y = 40;
constexpr int FALL_SPEED = 7;
constexpr float KEY_TIMING = 0.05f;
constexpr float TEXTURE_TETROMINO_SIZE = 8.0f;
constexpr float TEXTURE_TETROMINO_RATIO = 20.0f/8.0f;

class tetrisFloatBlock {
public:
    tetrisFloatBlock();
    tetrisFloatBlock(tetromino::tetrominoNames name,Rectangle* tetrisStage, tetrisControls* gameControls, Texture2D* tetrominoTexture);
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
    Texture2D* _tetrominoTexture;
};

class tetrisStaticBlocks {
public:
    tetrisStaticBlocks();
    tetrisStaticBlocks(tetrisEvent* eventPtr, Texture2D* tetrominoTexture);
    ~tetrisStaticBlocks();
    void Add(tetrisFloatBlock& tetrisBlock, tetromino::tetrominoNames tetrisName);
    void Display();
    void checkLine();
    const std::vector<Rectangle>& getRectangles();

private:
    std::map<float, int> updateLineMap();
    std::vector<Rectangle> _tetrisBlocks;
    std::vector<tetromino::tetrominoNames> _tetrisNames;
    std::map<float, int> _lineMap;
    tetrisEvent* _event;
    Texture2D* _tetrominoTexture;
};

#endif // HEADER_TETRIS_BLOCK