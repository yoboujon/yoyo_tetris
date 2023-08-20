#include "score.h"

tetrisScore::tetrisScore()
    : _score(0)
    , _scoreTime(0.0f)
{
}

tetrisScore::~tetrisScore()
{
}

void tetrisScore::updateScore()
{
    // Getting the frame time
    _scoreTime += GetFrameTime();
    // each second = 100 points.
    while (_scoreTime > 1.0f) {
        _score += 100;
        _scoreTime -= 1.0f;
    }
}

uint64_t tetrisScore::getScore() { return _score; }