#include "graphics/number_input.h"
#include "graphics/input_container.h"

#include <iostream>

TetrisInputNumber::TetrisInputNumber()
    : TetrisInputContainer({0},{0},[](int key){return false;})
{
}

TetrisInputNumber::TetrisInputNumber(Vector2 position, Size2 size)
    : TetrisInputContainer(position,size, [](int key){return key >= KEY_ZERO && key <= KEY_NINE;})
{
}

TetrisInputNumber::~TetrisInputNumber()
{
}

int TetrisInputNumber::GetInt() { return std::stoi(_text); }