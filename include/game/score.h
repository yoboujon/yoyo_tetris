#ifndef HEADER_TETRIS_SCORE
#define HEADER_TETRIS_SCORE

#include "lib.h"

#include "event/component.h"

class tetrisScore : public BaseComponent
{
public:
    tetrisScore();
    ~tetrisScore();
    void updateScore();
    void lineComplete();
    uint64_t getScore();
    void resetScore(bool display=false);
    uint8_t getMultiplicator();
private:
    uint64_t _score;
    uint8_t _multiplicator;
    bool _activeMultiplcator;
    float _multiplicatorTime;
    float _scoreTime;
};

#endif // HEADER_TETRIS_SCORE