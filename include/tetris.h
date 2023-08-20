#ifndef HEADER_TETRIS_GAME
#define HEADER_TETRIS_GAME

#include "lib.h"

#include "tetris_block.h"
#include "button.h"
#include "ui.h"
#include "score.h"

constexpr float FALLING_TICK_DURATION = 0.01f;

class tetrisGame {
public:
    tetrisGame(tetrisEvent* event, tetrisUI* gameUI, tetrisScore* gameScore);
    ~tetrisGame();
    void Loop();
    bool gameFinished();
    bool pause();
private:
    void DisplayTetrisNext();
    // Event pointer
    tetrisEvent* _eventPtr;

    // The controller/keyboard manager
    tetrisControls _gameControls;

    // Static and floating blocks of the game
    tetrisStaticBlocks _staticBlocks;
    tetrisFloatBlock* _actualBlock;
    tetrisFloatBlock* _nextBlock;

    // The next tetromino
    tetromino::tetrominoNames _actualName;

    // The UI pointer
    tetrisUI* _gameUI;

    // The score pointer
    tetrisScore* _gameScore;
    
    // The falling tick
    float _fallingTick;

    //Game States
    bool _isGameOver;
    bool _pauseMenu;
};

#endif // HEADER_TETRIS_GAME