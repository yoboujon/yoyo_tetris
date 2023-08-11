#include "tetris_block.h"
#include "raylib.h"
#include <iostream>

/* ========================== */
/*     FLOAT Tetris Block     */
/* ========================== */

floatTetrisBlock::floatTetrisBlock(Rectangle* screen)
    : _screen(screen)
    , _position({ BASE_X, BASE_Y })
{
    _object.push_back({ BASE_X, BASE_Y, BLOCK_SIZE, BLOCK_SIZE });
    _object.push_back({ BASE_X + BLOCK_SIZE, BASE_Y, BLOCK_SIZE, BLOCK_SIZE });
    _object.push_back({ BASE_X + BLOCK_SIZE, BASE_Y + BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE });
    _object.push_back({ BASE_X + BLOCK_SIZE, BASE_Y + (BLOCK_SIZE * 2), BLOCK_SIZE, BLOCK_SIZE });
    _height = 3 * BLOCK_SIZE;
}

floatTetrisBlock::~floatTetrisBlock()
{
}

void floatTetrisBlock::Move()
{
    for (auto& recVec : _object) {
        if (canMove())
            recVec.y = recVec.y + 1;
        DrawRectangleRec(recVec, BLUE);
    }
    if (canMove())
        _position.y = _position.y + 1;
}

bool floatTetrisBlock::canMove()
{
    for (auto& recVec : _object) {
        auto rectangleCollision = GetCollisionRec(recVec, *_screen);
        float area = rectangleCollision.width * rectangleCollision.height;
        if (area < 400) {
            return false;
        }
    };
    return true;
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
    return (_position.y == (496 - _height));
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
    for (auto& recVec : tetrisBlock.getRectangles()) {
        _tetrisblocks.push_back(recVec);
    }
}

void staticTetrisBlocks::Display()
{
    for (auto& staticBlocks : _tetrisblocks) {
        DrawRectangleRec(staticBlocks, BLUE);
    }
}