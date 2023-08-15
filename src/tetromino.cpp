#include "tetromino.h"
#include <random>

tetromino::tetrominoNames tetromino::getRandomTetromino()
{
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> randomName(0, static_cast<int>(tetromino::tetrominoNames::Count)-1);
    return static_cast<tetromino::tetrominoNames>(randomName(engine));
}