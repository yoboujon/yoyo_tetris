#include "tetromino.h"
#include <random>

const std::map<tetromino::tetrominoNames, tetromino::tetrominoBlock> tetrominoMap = {
    { tetromino::tetrominoNames::LightBlue_I, tetromino::tetrominoBlock(initArray({ 0, 1 }, { 3, 1 }), emptyArray(), SKYBLUE, { 1, 1 }) },
    { tetromino::tetrominoNames::Yellow_O, tetromino::tetrominoBlock(initArray({ 0, 0 }, { 1, 1 }), emptyArray(), GOLD, NULL_VECTOR2) },
    { tetromino::tetrominoNames::Purple_T, tetromino::tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 1, 0 }, { 1, 0 }), PURPLE, { 1, 1 }) },
    { tetromino::tetrominoNames::Green_S, tetromino::tetrominoBlock(initArray({ 0, 1 }, { 1, 1 }), initArray({ 1, 0 }, { 2, 0 }), GREEN, { 1, 1 }) },
    { tetromino::tetrominoNames::Red_Z, tetromino::tetrominoBlock(initArray({ 1, 1 }, { 2, 1 }), initArray({ 0, 0 }, { 1, 0 }), RED, { 1, 1 }) },
    { tetromino::tetrominoNames::Blue_J, tetromino::tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 0, 0 }, { 0, 0 }), BLUE, { 1, 1 }) },
    { tetromino::tetrominoNames::Orange_L, tetromino::tetrominoBlock(initArray({ 0, 1 }, { 2, 1 }), initArray({ 2, 0 }, { 2, 0 }), ORANGE, { 1, 1 }) }
};

inline Vector2 TETROMINO_MAP_RECT(tetromino::tetrominoNames name, int i, int num)
{
    return tetrominoMap.at(name).rectangles[i][num];
}

Color getColorTetromino(tetromino::tetrominoNames name)
{
    return tetrominoMap.at(name).color;
}

tetromino::tetrominoNames tetromino::getRandomTetromino()
{
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> randomName(0, static_cast<int>(tetromino::tetrominoNames::Count)-1);
    return static_cast<tetromino::tetrominoNames>(randomName(engine));
}

// Global functions for tetris

std::vector<Rectangle> constructReactangle(tetromino::tetrominoNames name, Vector2 position, float* area_object, tetromino::tetrisRotation rotation, bool calculateArea)
{
    std::vector<Rectangle> newObject;
    const auto floatRotate = (Vector2Equals(tetrominoMap.at(name).center, NULL_VECTOR2) ? 0 : getRotationAngle(rotation) * DEG_TO_RAD);
    Vector2 SquareSize { 1, 1 };

    for (int i = 0; i < 2; i++) {
        if (Vector2Equals(TETROMINO_MAP_RECT(name, i, 1), NULL_VECTOR2))
            continue;
        // Get the center of rotation.
        const Vector2 center = tetrominoMap.at(name).center;
        // We create separate variables to facilitate coordinate calculations.
        // Each vector is rotated based on the specified angle (90°, 180°, and 270°).
        // Substracting the center when rotating then adding it again to actually rotate around a point.
        const auto vectorStart = Vector2Add(Vector2Rotate(Vector2Subtract(TETROMINO_MAP_RECT(name, i, 0), center), floatRotate), center);
        const auto vectorEnd = Vector2Add(Vector2Rotate(Vector2Subtract(TETROMINO_MAP_RECT(name, i, 1), center), floatRotate), center);
        // const auto offsetRotate = getOffsetAngle(rotation,i);
        const auto xStart = round(vectorStart.x);
        const auto yStart = round(vectorStart.y);
        const auto xEnd = round(vectorEnd.x);
        const auto yEnd = round(vectorEnd.y);

        // Combining the current position  with the vector coordinates
        auto xObject = (xStart * BLOCK_SIZE) + position.x;
        auto yObject = (yStart * BLOCK_SIZE) + position.y;

        // If the width or height is negative, our calculations need to consider whether it's based on the end or start of the vector.
        // In Raylib, width and height cannot be negative, so we adjust the base position accordingly.
        if ((xEnd - xStart) < 0) {
            xObject = (xEnd * BLOCK_SIZE) + position.x;
            SquareSize.x = -1;
        }
        if ((yEnd - yStart) < 0) {
            yObject = (yEnd * BLOCK_SIZE) + position.y;
            SquareSize.y = -1;
        }

        // The formula sqrt((xb-xa)² + (yb-ya)² calculates the Euclidean distance between two vectors.
        // In this case, we're using the absolute difference in x or y coordinates, and then adding the size of a square since it's a 2D shape.
        const auto widthObject = abs(((xEnd - xStart) + SquareSize.x) * BLOCK_SIZE);
        const auto heightObject = abs(((yEnd - yStart) + SquareSize.y) * BLOCK_SIZE);

        // Adding the rectangle to the object and calculate area if needed.
        newObject.push_back({ xObject, yObject, widthObject, heightObject });
        if (calculateArea)
            *area_object += widthObject * heightObject;
    }
    return newObject;
}

int getRotationAngle(tetromino::tetrisRotation rotation)
{
    switch (rotation) {
    case tetromino::tetrisRotation::CLOCKWISE:
        return 90;
    case tetromino::tetrisRotation::INVERTED:
        return 180;
    case tetromino::tetrisRotation::COUNTER_CLOCKWISE:
        return 270;
    [[likely]] default:
        return 0;
    }
}