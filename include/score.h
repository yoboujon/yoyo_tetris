#ifndef HEADER_TETRIS_SCORE
#define HEADER_TETRIS_SCORE

#include "lib.h"
#include <stdint.h>

class tetrisScore {
public:
    tetrisScore();
    ~tetrisScore();
    void updateScore();
    uint64_t getScore();
private:
    uint64_t _score;
    float _scoreTime;
};

#endif // HEADER_TETRIS_SCORE