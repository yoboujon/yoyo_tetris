#include "tetris_block.h"
#include "controls.h"
#include "raymath.h"

#include <iostream>
#include <corecrt_math.h>
#include <stdint.h>
#include <vcruntime.h>

inline Vector2 TETROMINO_MAP_RECT(tetromino::tetrominoNames name, int i, int num)
{
    return tetrominoMap.at(name).rectangles[i][num];
}

using namespace tetromino;

const std::map<tetromino::tetrominoNames, tetromino::tetrominoBlock> tetrominoMap = {
    { tetrominoNames::LightBlue_I, tetrominoBlock(initArray({ 0, 1 }, { 3, 1 }), emptyArray(), SKYBLUE, { 1, 1 }) },
    { tetrominoNames::Yellow_O, tetrominoBlock(initArray({ 0, 0 }, { 1, 1 }), emptyArray(), GOLD, NULL_VECTOR2) },
    { tetrominoNames::Purple_T, tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 1, 0 }, { 1, 0 }), PURPLE, { 1, 1 }) },
    { tetrominoNames::Green_S, tetrominoBlock(initArray({ 0, 1 }, { 1, 1 }), initArray({ 1, 0 }, { 2, 0 }), GREEN, { 1, 1 }) },
    { tetrominoNames::Red_Z, tetrominoBlock(initArray({ 1, 1 }, { 2, 1 }), initArray({ 0, 0 }, { 1, 0 }), RED, { 1, 1 }) },
    { tetrominoNames::Blue_J, tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 0, 0 }, { 0, 0 }), BLUE, { 1, 1 }) },
    { tetrominoNames::Orange_L, tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 2, 0 }, { 2, 0 }), ORANGE, { 1, 1 }) }
};

/* ========================== */
/*     FLOAT Tetris Block     */
/* ========================== */

floatTetrisBlock::floatTetrisBlock(tetromino::tetrominoNames name, Rectangle* tetrisStage, controlsTetris* gameControls)
    : _name(name)
    , _color(tetrominoMap.at(name).color)
    , _position({ BASE_X, BASE_Y })
    , _placed(false)
    , _rotation(floatTetrisRotation::NONE)
    , _tetrisStage(tetrisStage)
    , _gameControls(gameControls)
{
    _area_object = 0;
    _object = constructReactangle(name);
}

floatTetrisBlock::~floatTetrisBlock()
{
}

void floatTetrisBlock::Fall(const std::vector<Rectangle>& tetrisBlock)
{
    std::vector<Rectangle> gameRectangleVec = { *_tetrisStage };
    int fallSpeed(1);
    if (_gameControls->IsKeyDown(KEY_DOWN))
        fallSpeed = 5;
    for (int i = 0; i < fallSpeed; i++) {
        auto newObject = moveY(1); // the speed
        const bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
        if (canBePlaced) {
            _placed = true;
        } else {
            _object = newObject;
            _position.y = _position.y + 1;
        }
        Display();
    }
}

void floatTetrisBlock::Move(const std::vector<Rectangle>& tetrisBlock)
{
    int offset(0);
    if (_gameControls->IsKeyDown(KEY_LEFT)) {
        offset = -BLOCK_SIZE;
    } else if (_gameControls->IsKeyDown(KEY_RIGHT)) {
        offset = BLOCK_SIZE;
    }
    std::vector<Rectangle> gameRectangleVec = { *_tetrisStage };
    auto newObject = moveX(offset); // the speed
    const bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
    if (!canBePlaced) {
        _object = newObject;
        _position.x = _position.x + offset;
    }
}

void floatTetrisBlock::Rotate(const std::vector<Rectangle>& tetrisBlock)
{
    std::vector<Rectangle> newRec;
    if (IsKeyPressed(KEY_UP)) {
        _rotation = (_rotation == floatTetrisRotation::COUNTER_CLOCKWISE ? floatTetrisRotation::NONE : static_cast<floatTetrisRotation>(static_cast<int>(_rotation) + 1));
        auto newObject = constructReactangle(_name, _rotation, false);
        const bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
        if (!canBePlaced) {
            _object = newObject;
        }
    }
}

void floatTetrisBlock::Display(void)
{
    for (auto& recVec : _object) {
        DrawRectangleRec(recVec, _color);
    }
}

bool floatTetrisBlock::checkGameRectangle(const std::vector<Rectangle>& newRectangles)
{
    float area(0);
    for (auto& newRect : newRectangles) {
        auto rectCollide = GetCollisionRec(newRect, *_tetrisStage);
        area += rectCollide.height * rectCollide.width;
    }
    return (area == _area_object);
}

bool floatTetrisBlock::checkCollisionWith(const std::vector<Rectangle>& newRectangles, const std::vector<Rectangle>& collideRectangles)
{
    for (auto& newRect : newRectangles) {
        for (auto& collideRect : collideRectangles) {
            if (CheckCollisionRecs(newRect, collideRect))
                return true;
        }
    }
    return false;
}

std::vector<Rectangle> floatTetrisBlock::moveX(int x)
{
    std::vector<Rectangle> newRec(_object);
    for (auto& rect : newRec) {
        rect.x = rect.x + x;
    }
    return newRec;
}

std::vector<Rectangle> floatTetrisBlock::moveY(int y)
{
    std::vector<Rectangle> newRec(_object);
    for (auto& rect : newRec) {
        rect.y = rect.y + y;
    }
    return newRec;
}

std::vector<Rectangle> floatTetrisBlock::constructReactangle(tetromino::tetrominoNames name, floatTetrisRotation rotation, bool calculateArea)
{
    std::vector<Rectangle> newObject;
    const auto floatRotate = (Vector2Equals(tetrominoMap.at(name).center, NULL_VECTOR2) ? 0 : getRotationAngle(rotation) * DEG_TO_RAD);
    Vector2 SquareSize { 1, 1 };

    for (int i = 0; i < 2; i++) {
        if (Vector2Equals(TETROMINO_MAP_RECT(name, i, 1), NULL_VECTOR2))
            continue;
        // Get the center of rotation.
        const Vector2 center = tetrominoMap.at(name).center;
        // We create separate variables to facilitate coordinate calculations.
        // Each vector is rotated based on the specified angle (90°, 180°, and 270°).
        // Substracting the center when rotating then adding it again to actually rotate around a point.
        const auto vectorStart = Vector2Add(Vector2Rotate(Vector2Subtract(TETROMINO_MAP_RECT(name, i, 0), center), floatRotate), center);
        const auto vectorEnd = Vector2Add(Vector2Rotate(Vector2Subtract(TETROMINO_MAP_RECT(name, i, 1), center), floatRotate), center);
        // const auto offsetRotate = getOffsetAngle(rotation,i);
        const auto xStart = round(vectorStart.x);
        const auto yStart = round(vectorStart.y);
        const auto xEnd = round(vectorEnd.x);
        const auto yEnd = round(vectorEnd.y);

        // Combining the current position  with the vector coordinates
        auto xObject = (xStart * BLOCK_SIZE) + _position.x;
        auto yObject = (yStart * BLOCK_SIZE) + _position.y;

        // If the width or height is negative, our calculations need to consider whether it's based on the end or start of the vector.
        // In Raylib, width and height cannot be negative, so we adjust the base position accordingly.
        if ((xEnd - xStart) < 0) {
            xObject = (xEnd * BLOCK_SIZE) + _position.x;
            SquareSize.x = -1;
        }
        if ((yEnd - yStart) < 0) {
            yObject = (yEnd * BLOCK_SIZE) + _position.y;
            SquareSize.y = -1;
        }

        // The formula sqrt((xb-xa)² + (yb-ya)² calculates the Euclidean distance between two vectors.
        // In this case, we're using the absolute difference in x or y coordinates, and then adding the size of a square since it's a 2D shape.
        const auto widthObject = abs(((xEnd - xStart) + SquareSize.x) * BLOCK_SIZE);
        const auto heightObject = abs(((yEnd - yStart) + SquareSize.y) * BLOCK_SIZE);

        // Adding the rectangle to the object and calculate area if needed.
        newObject.push_back({ xObject, yObject, widthObject, heightObject });
        if (calculateArea)
            _area_object += widthObject * heightObject;
    }
    return newObject;
}

int floatTetrisBlock::getRotationAngle(floatTetrisRotation rotation)
{
    switch (rotation) {
    case floatTetrisRotation::CLOCKWISE:
        return 90;
    case floatTetrisRotation::INVERTED:
        return 180;
    case floatTetrisRotation::COUNTER_CLOCKWISE:
        return 270;
    //[[likely]]
    default:
        return 0;
    }
}

Rectangle* floatTetrisBlock::getRectangle(int index)
{
    return &(_object.at(index));
}

const std::vector<Rectangle>& floatTetrisBlock::getRectangles()
{
    return _object;
}

bool floatTetrisBlock::Placed()
{
    return _placed;
}

tetromino::tetrominoNames floatTetrisBlock::getName()
{
    return _name;
}

Color floatTetrisBlock::getColor()
{
    return _color;
}

bool floatTetrisBlock::GameEnded(const std::vector<Rectangle>& tetrisBlock)
{
    return checkCollisionWith(tetrisBlock, _object);
}

/* ========================== */
/*    STATIC Tetris Blocks    */
/* ========================== */

staticTetrisBlocks::staticTetrisBlocks()
{
}

staticTetrisBlocks::~staticTetrisBlocks()
{
}

void staticTetrisBlocks::Add(floatTetrisBlock& tetrisBlock, Color tetrisColor)
{
    Vector2 increment, otherIncrement;
    float RectangleSize, OtherSize;
    for (auto recVec : tetrisBlock.getRectangles()) {
        if (recVec.width > recVec.height) {
            increment = { 1, 0 };
            otherIncrement = { 0, 1 };
            RectangleSize = recVec.width / BLOCK_SIZE;
            OtherSize = recVec.height / BLOCK_SIZE;
        } else {
            increment = { 0, 1 };
            otherIncrement = { 1, 0 };
            RectangleSize = recVec.height / BLOCK_SIZE;
            OtherSize = recVec.width / BLOCK_SIZE;
        }
        for (float i = 0.0f; i < OtherSize; i++) {
            for (float j = 0.0f; j < RectangleSize; j++) {
                const auto yCoord = (recVec.y + (j * increment.y * BLOCK_SIZE) + (i * otherIncrement.y * BLOCK_SIZE));
                _tetrisBlocks.push_back({ (recVec.x + (j * increment.x * BLOCK_SIZE) + (i * otherIncrement.x * BLOCK_SIZE)),
                    yCoord,
                    BLOCK_SIZE, BLOCK_SIZE });
                _tetrisColors.push_back(tetrisColor);
                if (_lineMap.find(yCoord) == _lineMap.end()) {
                    _lineMap[yCoord] = 1;
                } else {
                    _lineMap[yCoord] += 1;
                }
            }
        }
    }
}

void staticTetrisBlocks::Display()
{
    for (size_t i = 0; i < _tetrisBlocks.size(); i++) {
        DrawRectangleRec(_tetrisBlocks.at(i), _tetrisColors.at(i));
    }
}

void staticTetrisBlocks::checkLine()
{
    for (auto it = _lineMap.begin(); it != _lineMap.end();)
    {
        if(it->second == 15)    // For now 15 is the size of game area
        {
            // TODO : Play animation
            for (size_t i=0; i < _tetrisBlocks.size();)
            {
                if(_tetrisBlocks.at(i).y == it->first)
                {
                    _tetrisBlocks.erase(_tetrisBlocks.begin() + i);
                    _tetrisColors.erase(_tetrisColors.begin() + i);
                    i = 0;
                }
                else
                    i++;
            }
            // For every blocks that are higher than the destroyed block, update its position to fall.
            // Reseting the linemap for the selected y coordonate
            for (auto& block :_tetrisBlocks)
            {
                if(block.y < it->first)
                    block.y = block.y + BLOCK_SIZE;
            }
            _lineMap = updateLineMap();
            it = _lineMap.begin();
        }
        else {
            it++;
        }
    }
}

std::map<float, int> staticTetrisBlocks::updateLineMap()
{
    std::map<float, int> newMap;
    for(const auto& block : _tetrisBlocks)
    {
        if (newMap.find(block.y) == newMap.end()) {
            newMap[block.y] = 1;
        } else {
            newMap[block.y] += 1;
        }
    }
    return newMap;
}

const std::vector<Rectangle>& staticTetrisBlocks::getRectangles()
{
    return _tetrisBlocks;
}