#ifndef HEADER_LIBRARY
#define HEADER_LIBRARY

#include "raylib.h"

#include <array>

using VectorMaths = std::array<Vector2, 2>;

VectorMaths initArray(Vector2 a, Vector2 b);
VectorMaths emptyArray();

constexpr Vector2 NULL_VECTOR2 = {-1,-1};
constexpr int SCREEN_WIDTH = 760;
constexpr int SCREEN_HEIGHT = 540;
constexpr Color PAUSE_COLOR = { 39, 40, 41, 120 };

#endif // HEADER_LIBRARY