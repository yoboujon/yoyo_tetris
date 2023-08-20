#ifndef HEADER_TETRIS_SCORE
#define HEADER_TETRIS_SCORE

#include "lib.h"
#include "event.h"

class tetrisScore {
public:
    tetrisScore(tetrisEvent* eventPtr);
    ~tetrisScore();
    void updateScore();
    uint64_t getScore();
    uint8_t getMultiplicator();
private:
    tetrisEvent* _event;
    uint64_t _score;
    uint8_t _multiplicator;
    bool _activeMultiplcator;
    float _multiplicatorTime;
    float _scoreTime;
};

#endif // HEADER_TETRIS_SCORE