#include "lib.h"

VectorMaths initArray(Vector2 a, Vector2 b)
{
    return VectorMaths{ a, b };
}

VectorMaths emptyArray()
{
    return VectorMaths{ Vector2{-1, -1},Vector2{-1, -1} };
}