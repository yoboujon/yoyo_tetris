#ifndef HEADER_TETROMINO
#define HEADER_TETROMINO

#include "lib.h"
#include <map>

namespace tetromino {

enum class tetrominoNames {
    LightBlue_I,
    Yellow_O,
    Purple_T,
    Green_S,
    Red_Z,
    Blue_J,
    Orange_L
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

};

extern const std::map<tetromino::tetrominoNames, tetromino::tetrominoBlock> tetrominoMap;

#endif // HEADER_TETROMINO