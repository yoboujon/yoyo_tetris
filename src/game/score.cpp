#include "game/score.h"

#include <iostream>

tetrisScore::tetrisScore()
    : _score(0), _multiplicator(1), _activeMultiplcator(false), _multiplicatorTime(0.0f), _scoreTime(0.0f)
{
}

tetrisScore::~tetrisScore()
{
}

void tetrisScore::resetScore(bool display)
{
    _score = 0;
    _multiplicator = 1;
    _activeMultiplcator = false;
    _multiplicatorTime = 0.0f;
    _scoreTime = 0.0f;
    if(display)
    {
        this->_eventHandler->sendEvent(this, EventType::SEND_SCORE, _score);
        this->_eventHandler->sendEvent(this, EventType::SEND_MULTIPLICATOR, _multiplicator);
    }
}

void tetrisScore::updateScore()
{
    // Getting the frame time
    const float frameTime(GetFrameTime());
    _scoreTime += frameTime;
    _multiplicatorTime += frameTime;

    // each second = 100 points.
    while (_scoreTime > 1.0f)
    {
        _score += 100;
        _scoreTime -= 1.0f;
        this->_eventHandler->sendEvent(this, EventType::SEND_SCORE, _score);
    }
    if (_activeMultiplcator && (_multiplicatorTime >= 10.0f))
    {
        _multiplicatorTime = 0.0f;
        _multiplicator = 1;
        _activeMultiplcator = false;
        this->_eventHandler->sendEvent(this, EventType::SEND_MULTIPLICATOR, _multiplicator);
    }
}

void tetrisScore::lineComplete()
{
    // each line = 1000 points*multiplicator
    // If a line has already been completed, multiple gets x2
    // the period between each line has to be less than 10 seconds
    if (_activeMultiplcator && (_multiplicatorTime < 10.0f))
    {
        _multiplicator = (_multiplicator == 0x80 ? UINT8_MAX : _multiplicator);
        _multiplicator = (_multiplicator <= 0x80 ? _multiplicator *= 2 : _multiplicator);
    }
    _score += 1000 * _multiplicator;
    _activeMultiplcator = true;
    _multiplicatorTime = 0.0f;

    this->_eventHandler->sendEvent(this, EventType::SEND_SCORE, _score);
    this->_eventHandler->sendEvent(this, EventType::SEND_MULTIPLICATOR, _multiplicator);
}

uint64_t tetrisScore::getScore() { return _score; }
uint8_t tetrisScore::getMultiplicator() { return _multiplicator; }