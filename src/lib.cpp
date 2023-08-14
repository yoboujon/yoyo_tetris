#include "lib.h"

VectorMaths initArray(Vector2 a, Vector2 b)
{
    return VectorMaths{ a, b };
}

VectorMaths emptyArray()
{
    return VectorMaths{ NULL_VECTOR2, NULL_VECTOR2 };
}