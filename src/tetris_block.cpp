#include "tetris_block.h"
#include <iostream>

tetrisBlock::tetrisBlock() : _position({BASE_X,BASE_Y})
{
    _object.push_back({BASE_X,BASE_Y,BLOCK_SIZE,BLOCK_SIZE});
    _object.push_back({BASE_X+BLOCK_SIZE,BASE_Y,BLOCK_SIZE,BLOCK_SIZE});
    _object.push_back({BASE_X+BLOCK_SIZE,BASE_Y+BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE});
    _object.push_back({BASE_X+BLOCK_SIZE,BASE_Y+(BLOCK_SIZE*2),BLOCK_SIZE,BLOCK_SIZE});
    _height = 3*BLOCK_SIZE;
}

tetrisBlock::~tetrisBlock()
{

}

void tetrisBlock::Move()
{
    for(auto& recVec:_object)
    {
        if (canMove()) recVec.y = recVec.y+1;
        DrawRectangleRec(recVec, BLUE);
    }
    if (canMove()) _position.y = _position.y + 1;
}

bool tetrisBlock::canMove()
{
    return (_position.y != (496 - _height));
}