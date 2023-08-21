#ifndef HEADER_LIBRARY
#define HEADER_LIBRARY

#include "raylib.h"
#include "raymath.h"

#include <stdint.h>
#include <array>

using VectorMaths = std::array<Vector2, 2>;

VectorMaths initArray(Vector2 a, Vector2 b);
VectorMaths emptyArray();
void DrawTextureRatio(Texture2D texture, Vector2 source, Rectangle destination, float ratio, Vector2 position, Color tint);
bool checkCollisionPointRecArray(Vector2 point, Rectangle* rectArray, int sizeArray);

// Used for the tetromino map
constexpr Vector2 NULL_VECTOR2 = {-1,-1};
// TODO : Capability to change the size of the window
constexpr int SCREEN_WIDTH = 697;
constexpr int SCREEN_HEIGHT = 488;
// * The size of one block.
constexpr int BLOCK_SIZE = 20;
// 10*20 is the standard tetris grid size.
// TODO : Multiple game modes with different sizes.
constexpr int HORIZONTAL_GRID_SIZE = 10;
constexpr int VERTICAL_GRID_SIZE = 20;
// The stage, x and y can be changed, the width and height corresponds to the 10*20 ratio of any tetris game.
// ! +6 for the height to correspond perfectly to the tile
constexpr Rectangle TETRIS_STAGE = {246, 40, BLOCK_SIZE*HORIZONTAL_GRID_SIZE, BLOCK_SIZE*VERTICAL_GRID_SIZE+6};

constexpr Color PAUSE_COLOR = { 39, 40, 41, 120 };

// Size2, 2 components
typedef struct Size2 {
    float width;                // Size w component
    float height;               // Size h component
} Size2;

#endif // HEADER_LIBRARY