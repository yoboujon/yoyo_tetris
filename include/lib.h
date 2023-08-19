#ifndef HEADER_LIBRARY
#define HEADER_LIBRARY

#include "raylib.h"
#include "raymath.h"

#include <array>

using VectorMaths = std::array<Vector2, 2>;

VectorMaths initArray(Vector2 a, Vector2 b);
VectorMaths emptyArray();
void DrawTextureRatio(Texture2D texture, Vector2 source, Rectangle destination, float ratio, Vector2 position, Color tint);
bool checkCollisionPointRecArray(Vector2 point, Rectangle* rectArray, int sizeArray);

constexpr Vector2 NULL_VECTOR2 = {-1,-1};
constexpr int SCREEN_WIDTH = 760;
constexpr int SCREEN_HEIGHT = 540;
constexpr Color PAUSE_COLOR = { 39, 40, 41, 120 };

// Size2, 2 components
typedef struct Size2 {
    float width;                // Size w component
    float height;               // Size h component
} Size2;

#endif // HEADER_LIBRARY