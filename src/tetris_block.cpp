#include "tetris_block.h"
#include "raylib.h"
#include <iostream>
#include <stdint.h>

/* ========================== */
/*     FLOAT Tetris Block     */
/* ========================== */

floatTetrisBlock::floatTetrisBlock(Rectangle* gameRectangle)
    : _placed(false)
    , _gameRectangle(gameRectangle)
    , _timingCommand(0.0f)
    , _elapsed(0.0f)
{
    _area_object = 0;
    _object.push_back({ BASE_X, BASE_Y, BLOCK_SIZE, BLOCK_SIZE });
    _area_object += BLOCK_SIZE * BLOCK_SIZE;
    _object.push_back({ BASE_X + BLOCK_SIZE, BASE_Y, BLOCK_SIZE, BLOCK_SIZE * 3 });
    _area_object += BLOCK_SIZE * (BLOCK_SIZE * 3);
}

floatTetrisBlock::~floatTetrisBlock()
{
}

void floatTetrisBlock::Fall(const std::vector<Rectangle>& tetrisBlock)
{
    std::vector<Rectangle> gameRectangleVec = { *_gameRectangle };
    int fallSpeed(1);
    if (IsKeyDown(KEY_DOWN))
        fallSpeed = 5;
    for(int i=0; i<fallSpeed; i++)
    {
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
    _elapsed += GetFrameTime();
    if(_elapsed <= _timingCommand) return;
    if (IsKeyDown(KEY_LEFT))
    {
        offset = -BLOCK_SIZE;
        _timingCommand = _elapsed + 0.025;
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        offset = BLOCK_SIZE;
        _timingCommand = _elapsed + 0.025;
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
        DrawRectangleRec(recVec, BLUE);
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

/* ========================== */
/*    STATIC Tetris Blocks    */
/* ========================== */

staticTetrisBlocks::staticTetrisBlocks()
{
}

staticTetrisBlocks::~staticTetrisBlocks()
{
}

void staticTetrisBlocks::Add(floatTetrisBlock& tetrisBlock)
{
    for (auto recVec : tetrisBlock.getRectangles()) {
        _tetrisblocks.push_back(recVec);
    }
}

void staticTetrisBlocks::Display()
{
    for (auto& staticBlocks : _tetrisblocks) {
        DrawRectangleRec(staticBlocks, BLUE);
    }
}

const std::vector<Rectangle>& staticTetrisBlocks::getRectangles()
{
    return _tetrisblocks;
}