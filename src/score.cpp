#include "score.h"

#include <iostream>

tetrisScore::tetrisScore(tetrisEvent* eventPtr)
    : _event(eventPtr)
    , _score(0)
    , _multiplicator(1)
    , _activeMultiplcator(false)
    , _multiplicatorTime(0.0f)
    , _scoreTime(0.0f)
{
}

tetrisScore::~tetrisScore()
{
}

void tetrisScore::updateScore()
{
    // Getting the frame time
    const float frameTime(GetFrameTime());
    _scoreTime += frameTime;
    _multiplicatorTime += frameTime;

    // each second = 100 points.
    while (_scoreTime > 1.0f) {
        _score += 100;
        _scoreTime -= 1.0f;
    }
    if (_activeMultiplcator && (_multiplicatorTime >= 10.0f)) {
        _multiplicatorTime = 0.0f;
        _multiplicator = 1;
        _activeMultiplcator = false;
    }

    // each line = 1000 points*multiplicator
    if (_event->OnEvent(eventType::TETRIS_LINE_COMPLETED, eventUser::SCORE)) {
        // If a line has already been completed, multiple gets x2
        // the period between each line has to be less than 10 seconds
        if (_activeMultiplcator && (_multiplicatorTime < 10.0f))
            _multiplicator = (_multiplicator < UINT8_MAX ? _multiplicator *= 2 : _multiplicator);
        _score += 1000 * _multiplicator;
        _activeMultiplcator = true;
        _multiplicatorTime = 0.0f;
    }
}

uint64_t tetrisScore::getScore() { return _score; }
uint8_t tetrisScore::getMultiplicator() { return _multiplicator; }