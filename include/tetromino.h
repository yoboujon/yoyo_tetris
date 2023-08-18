#ifndef HEADER_TETROMINO
#define HEADER_TETROMINO

#include "lib.h"
#include <map>
#include <vector>

constexpr double DEG_TO_RAD = PI/180;
constexpr int BLOCK_SIZE = 20;

namespace tetromino {

enum class tetrominoNames {
    LightBlue_I,
    Yellow_O,
    Purple_T,
    Green_S,
    Red_Z,
    Blue_J,
    Orange_L,
    Count
};

enum class tetrisRotation {
    NONE,
    CLOCKWISE,
    INVERTED,
    COUNTER_CLOCKWISE
};

struct tetrominoBlock {
    std::array<VectorMaths, 2> rectangles;
    Color color;
    Vector2 center;

    // Constructor
    constexpr tetrominoBlock(const VectorMaths& rect1, const VectorMaths& rect2, const Color& c, const Vector2& cVect)
        : rectangles({ rect1, rect2 })
        , color(c)
        , center(cVect)
    {
    }
};

tetromino::tetrominoNames getRandomTetromino();

};

std::vector<Rectangle> constructReactangle(tetromino::tetrominoNames name, Vector2 position, float* area_object, tetromino::tetrisRotation rotation=tetromino::tetrisRotation::NONE, bool calculateArea=false);
Color getColorTetromino(tetromino::tetrominoNames name);
int getRotationAngle(tetromino::tetrisRotation rotation);

#endif // HEADER_TETROMINO