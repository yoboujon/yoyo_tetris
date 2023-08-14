#ifndef HEADER_LIBRARY
#define HEADER_LIBRARY

#include "raylib.h"
//#include "raymath.h"
#include <array>

using VectorMaths = std::array<Vector2, 2>;

VectorMaths initArray(Vector2 a, Vector2 b);
VectorMaths emptyArray();

constexpr Vector2 NULL_VECTOR2 = {-1,-1};

#endif // HEADER_LIBRARY