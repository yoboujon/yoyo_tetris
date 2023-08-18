#ifndef HEADER_TETRIS_GAME
#define HEADER_TETRIS_GAME

#include "lib.h"
#include "tetris_block.h"
#include "button.h"
#include "ui.h"

constexpr float FALLING_TICK_DURATION = 0.01f;

class gameTetris {
public:
    gameTetris(tetrisUI* gameUI, tetromino::tetrominoNames name);
    ~gameTetris();
    void Loop();
    bool gameFinished();
private:
    void DisplayTetrisNext();
    // The controller/keyboard manager
    controlsTetris _gameControls;
    // Static and floating blocks of the game
    staticTetrisBlocks _staticBlocks;
    floatTetrisBlock* _actualBlock;
    floatTetrisBlock* _nextBlock;
    // The next tetromino
    tetromino::tetrominoNames _actualName;
    // The UI pointer
    tetrisUI* _gameUI;
    // The falling tick
    float _fallingTick;

    //Game States
    bool _isGameOver;
};

#endif // HEADER_TETRIS_GAME