#include "tetris_block.h"
#include "controls.h"
#include <iostream>
#include <stdint.h>
#include <vcruntime.h>

#define TETROMINO_MAP_RECT(name, i, num) tetrominoMap.at(name).rectangles.at(i).at(num)

using namespace tetromino;

const std::map<tetromino::tetrominoNames, tetromino::tetrominoBlock> tetrominoMap = {
    { tetrominoNames::LightBlue_I, tetrominoBlock(initArray({ 0, 1 }, { 3, 1 }), emptyArray(), SKYBLUE) },
    { tetrominoNames::Yellow_O, tetrominoBlock(initArray({ 0, 0 }, { 1, 1 }), emptyArray(), GOLD) },
    { tetrominoNames::Purple_T, tetrominoBlock(initArray({ 0, 0 }, { 0, 0 }), initArray({ 10, 20 }, { 10, 20 }), PURPLE) },
    { tetrominoNames::Green_S, tetrominoBlock(initArray({ 0, 0 }, { 0, 0 }), initArray({ 10, 20 }, { 10, 20 }), GREEN) },
    { tetrominoNames::Red_Z, tetrominoBlock(initArray({ 0, 0 }, { 0, 0 }), initArray({ 10, 20 }, { 10, 20 }), RED) },
    { tetrominoNames::Blue_J, tetrominoBlock(initArray({ 0, 0 }, { 0, 0 }), initArray({ 10, 20 }, { 10, 20 }), BLUE) },
    { tetrominoNames::Orange_L, tetrominoBlock(initArray({ 0, 0 }, { 0, 0 }), initArray({ 10, 20 }, { 10, 20 }), ORANGE) }
};

/* ========================== */
/*     FLOAT Tetris Block     */
/* ========================== */

floatTetrisBlock::floatTetrisBlock(tetromino::tetrominoNames name, Rectangle* gameRectangle, controlsTetris* gameControls)
    : _name(name)
    , _placed(false)
    , _gameRectangle(gameRectangle)
    , _gameControls(gameControls)
    , _color(tetrominoMap.at(name).color)
{
    std::cout << "tetrominoMap size: " << tetrominoMap.size() << std::endl;
    _area_object = 0;
    for (int i = 0; i < 2; i++) {
        if (TETROMINO_MAP_RECT(name, i, 1).x == -1)
            continue;
        auto widthObject = ((TETROMINO_MAP_RECT(name, i, 1).x - TETROMINO_MAP_RECT(name, i, 0).x) + 1) * BLOCK_SIZE;
        auto heightObject = ((TETROMINO_MAP_RECT(name, i, 1).y - TETROMINO_MAP_RECT(name, i, 0).y) + 1) * BLOCK_SIZE;
        _object.push_back({ BASE_X, BASE_Y, widthObject, heightObject });
        std::cout << "TETROMINO_MAP_RECT: 1: " << TETROMINO_MAP_RECT(name, i, 1).x
                  << ", " << TETROMINO_MAP_RECT(name, i, 1).y
                  << "\t0: " << TETROMINO_MAP_RECT(name, i, 0).x
                  << ", " << TETROMINO_MAP_RECT(name, i, 0).y;
        _area_object += widthObject * heightObject;
    }
}

floatTetrisBlock::~floatTetrisBlock()
{
}

void floatTetrisBlock::Fall(const std::vector<Rectangle>& tetrisBlock)
{
    std::vector<Rectangle> gameRectangleVec = { *_gameRectangle };
    int fallSpeed(1);
    if (_gameControls->IsKeyDown(KEY_DOWN))
        fallSpeed = 5;
    for (int i = 0; i < fallSpeed; i++) {
        auto newObject = moveY(1); // the speed
        bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
        if (canBePlaced)
            _placed = true;
        else
            _object = newObject;
        printRec();
    }
}

void floatTetrisBlock::Move(const std::vector<Rectangle>& tetrisBlock)
{
    int offset(0);
    if (_gameControls->IsKeyDownTiming(KEY_LEFT, 0.075f)) {
        offset = -BLOCK_SIZE;
    } else if (_gameControls->IsKeyDownTiming(KEY_RIGHT, 0.075f)) {
        offset = BLOCK_SIZE;
    }
    std::vector<Rectangle> gameRectangleVec = { *_gameRectangle };
    auto newObject = moveX(offset); // the speed
    bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
    if (!canBePlaced)
        _object = newObject;
    printRec();
}

void floatTetrisBlock::printRec(void)
{
    for (auto& recVec : _object) {
        DrawRectangleRec(recVec, _color);
    }
}

bool floatTetrisBlock::checkGameRectangle(const std::vector<Rectangle>& newRectangles)
{
    float area(0);
    for (auto& newRect : newRectangles) {
        auto rectCollide = GetCollisionRec(newRect, *_gameRectangle);
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
    for (auto recVec : tetrisBlock.getRectangles()) {
        _tetrisBlocks.push_back(recVec);
    }
    _tetrisColors.push_back(tetrisColor);
}

void staticTetrisBlocks::Display()
{
    for (size_t i=0; i<_tetrisBlocks.size(); i++) {
        DrawRectangleRec(_tetrisBlocks.at(i), _tetrisColors.at(i));
    }
}

const std::vector<Rectangle>& staticTetrisBlocks::getRectangles()
{
    return _tetrisBlocks;
}