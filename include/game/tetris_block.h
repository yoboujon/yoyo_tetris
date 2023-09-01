#ifndef HEADER_TETRIS_BLOCK
#define HEADER_TETRIS_BLOCK

#include "lib.h"

#include "event/component.h"
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

class tetrisFloatBlock : public BaseComponent
{
public:
    tetrisFloatBlock();
    tetrisFloatBlock(tetromino::tetrominoNames name, Rectangle tetrisStage, tetrisControls *gameControls);
    ~tetrisFloatBlock();
    // Movement
    void Fall(const std::vector<Rectangle> &tetrisBlock);
    void Move(const std::vector<Rectangle> &tetrisBlock);
    void Rotate(const std::vector<Rectangle> &tetrisBlock);
    // Getter
    bool Placed();
    Rectangle *getRectangle(int index);
    const std::vector<Rectangle> &getRectangles();
    Color getColor();
    tetromino::tetrominoNames getName();
    bool GameEnded(const std::vector<Rectangle> &tetrisBlock);
    void Display();
    void DisplayNext();
    // Setter
    void setTextures(std::vector<Texture2D> texture);

private:
    bool checkCollisionWith(const std::vector<Rectangle> &newRectangles, const std::vector<Rectangle> &collideRectangles);
    bool checkGameRectangle(const std::vector<Rectangle> &newRectangles);
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
    Rectangle _tetrisStage;
    tetrisControls *_gameControls;
    std::vector<Texture2D> _tetrominoTextureVect;
};

class tetrisStaticBlocks : public BaseComponent
{
public:
    tetrisStaticBlocks();
    ~tetrisStaticBlocks();
    void Add(tetrisFloatBlock &tetrisBlock, tetromino::tetrominoNames tetrisName);
    void Display();
    void checkLine();
    const std::vector<Rectangle> &getRectangles();
    void setTexture(Texture2D texture);

private:
    std::map<float, int> updateLineMap();
    std::vector<Rectangle> _tetrisBlocks;
    std::vector<tetromino::tetrominoNames> _tetrisNames;
    std::map<float, int> _lineMap;
    Texture2D _tetrominoTexture;
};

#endif // HEADER_TETRIS_BLOCK