#include "tetris_block.h"
#include "controls.h"
#include "raylib.h"
#include "raymath.h"

#include <corecrt_math.h>
#include <iostream>
#include <stdint.h>
#include <vcruntime.h>

inline Vector2 TETROMINO_MAP_RECT(tetromino::tetrominoNames name, int i, int num)
{
    return tetrominoMap.at(name).rectangles[i][num];
}

using namespace tetromino;

const std::map<tetromino::tetrominoNames, tetromino::tetrominoBlock> tetrominoMap = {
    { tetrominoNames::LightBlue_I, tetrominoBlock(initArray({ 0, 1 }, { 3, 1 }), emptyArray(), SKYBLUE) },
    { tetrominoNames::Yellow_O, tetrominoBlock(initArray({ 0, 0 }, { 1, 1 }), emptyArray(), GOLD) },
    { tetrominoNames::Purple_T, tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 1, 0 }, { 1, 0 }), PURPLE) },
    { tetrominoNames::Green_S, tetrominoBlock(initArray({ 0, 1 }, { 1, 1 }), initArray({ 1, 0 }, { 2, 0 }), GREEN) },
    { tetrominoNames::Red_Z, tetrominoBlock(initArray({ 0, 0 }, { 1, 0 }), initArray({ 1, 1 }, { 2, 1 }), RED) },
    { tetrominoNames::Blue_J, tetrominoBlock(initArray({ 0, 0 }, { 0, 0 }), initArray({ 0, 1 }, { 2, 1 }), BLUE) },
    { tetrominoNames::Orange_L, tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 2, 0 }, { 2, 0 }), ORANGE) }
};

/* ========================== */
/*     FLOAT Tetris Block     */
/* ========================== */

floatTetrisBlock::floatTetrisBlock(tetromino::tetrominoNames name, Rectangle* gameRectangle, controlsTetris* gameControls)
    : _name(name)
    , _color(tetrominoMap.at(name).color)
    , _position({ BASE_X, BASE_Y })
    , _placed(false)
    , _rotation(floatTetrisRotation::NONE)
    , _gameRectangle(gameRectangle)
    , _gameControls(gameControls)
{
    _area_object = 0;
    constructReactangle(name);
    Vector2 originalVector = {2, 0};
    Vector2 rotate = Vector2Rotate(originalVector, (3*PI)/2);   // * COUNTER_CLOCKWISE (270)
    rotate = Vector2Rotate(originalVector, PI);             // * INVERTED
    rotate = Vector2Rotate(originalVector, (PI)/2);      // * CLOCKWISE
    std::cout << "x: " << round(rotate.x) << "\ty: " << round(rotate.y) << std::endl;
    //std::cout << "<NONE> x: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), 0).x << "\ty: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), 0).y << std::endl;
    //std::cout << "<CLOCKWISE> x: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), -90).x << "\ty: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), -90).y << std::endl;
    //std::cout << "<INVERTED> x: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), -180).x << "\ty: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), -180).y << std::endl;
    //std::cout << "<COUNTER_CLOCKWISE> x: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), -270).x << "\ty: " << Vector2Rotate(TETROMINO_MAP_RECT(name,0,0), -270).y << std::endl;
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
        if (canBePlaced) {
            _placed = true;
        } else {
            _object = newObject;
            _position.y = _position.y + 1;
        }
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
    if (!canBePlaced) {
        _object = newObject;
        _position.x = _position.x + offset;
    }
    printRec();
}

void floatTetrisBlock::Rotate()
{
    std::vector<Rectangle> newRec;
    if (IsKeyPressed(KEY_UP)) {
        _rotation = (_rotation == floatTetrisRotation::COUNTER_CLOCKWISE ?  floatTetrisRotation::NONE : static_cast<floatTetrisRotation>(static_cast<int>(_rotation) + 90));
        std::cout << "rotation: " << static_cast<int>(_rotation) << std::endl;
        constructReactangle(_name, _rotation, false);
    }
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

void floatTetrisBlock::constructReactangle(tetromino::tetrominoNames name, floatTetrisRotation rotation, bool calculateArea)
{
    std::vector<Rectangle> newObject;
    double floatRotate = static_cast<int>(rotation)*DEG_TO_RAD;
    std::cout << "floatRotate: " << floatRotate << std::endl;
    for (int i = 0; i < 2; i++) {
        if (TETROMINO_MAP_RECT(name, i, 1).x == -1)
            continue;
        auto vectorStart = Vector2Rotate( (TETROMINO_MAP_RECT(name, i, 0)), floatRotate);
        auto vectorEnd = Vector2Rotate( (TETROMINO_MAP_RECT(name, i, 1)), floatRotate);
        auto widthObject = (abs(round(vectorEnd.x)) - abs(round(vectorStart.x)) + 1) * BLOCK_SIZE;
        auto heightObject = (abs(round(vectorEnd.y)) - abs(round(vectorStart.y)) + 1) * BLOCK_SIZE;
        auto xObject = _position.x + (round(vectorStart.x) * BLOCK_SIZE);
        auto yObject = _position.y + (round(vectorStart.y) * BLOCK_SIZE);
        /*
        std::cout << "<VectorStart> x: " <<  vectorStart.x << "\ty: " << vectorStart.y << std::endl;
        std::cout << "<vectorEnd> x: " <<  vectorEnd.x << "\ty: " << vectorEnd.y << std::endl;
        */
        newObject.push_back({ xObject, yObject, widthObject, heightObject });
        if (calculateArea)
            _area_object += widthObject * heightObject;
    }
    _object = newObject;
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
        _tetrisColors.push_back(tetrisColor);
    }
}

void staticTetrisBlocks::Display()
{
    for (size_t i = 0; i < _tetrisBlocks.size(); i++) {
        DrawRectangleRec(_tetrisBlocks.at(i), _tetrisColors.at(i));
    }
}

const std::vector<Rectangle>& staticTetrisBlocks::getRectangles()
{
    return _tetrisBlocks;
}