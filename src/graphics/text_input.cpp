#include "graphics/text_input.h"
#include "graphics/input_container.h"

#include <iostream>

TetrisInputText::TetrisInputText()
    : TetrisInputContainer({0},{0},[](int key){return false;})
{
}

TetrisInputText::TetrisInputText(Vector2 position, Size2 size)
    : TetrisInputContainer(position,size, [](int key){return key > KEY_NULL && key < KEY_ESCAPE;})
{
}

TetrisInputText::~TetrisInputText()
{
}

std::string TetrisInputText::GetText() { return _text; }