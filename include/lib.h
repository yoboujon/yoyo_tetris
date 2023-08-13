#ifndef HEADER_LIBRARY
#define HEADER_LIBRARY

#include "raylib.h"
#include <array>

using VectorMaths = std::array<Vector2, 2>;

VectorMaths initArray(Vector2 a, Vector2 b);
VectorMaths emptyArray();

#endif // HEADER_LIBRARY