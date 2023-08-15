#ifndef HEADER_TETRIS_GAME
#define HEADER_TETRIS_GAME

#include "lib.h"
#include "tetris_block.h"

class gameTetris {
public:
    gameTetris(Rectangle* gameRectangle, tetromino::tetrominoNames name);
    ~gameTetris();
    void Loop();
    bool gameFinished();
    void GameOverScreen();
private:
    // The controller/keyboard manager
    controlsTetris _gameControls;
    // Static and floating blocks of the game
    staticTetrisBlocks _staticBlocks;
    floatTetrisBlock* _actualBlock;
    // The next tetromino
    tetromino::tetrominoNames _actualName;
    // The actual game space
    Rectangle* _gameRectangle;

    bool _gameOver;
};

#endif // HEADER_TETRIS_GAME