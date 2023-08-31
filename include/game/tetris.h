#ifndef HEADER_TETRIS_GAME
#define HEADER_TETRIS_GAME

#include "lib.h"

#include "game/tetris_block.h"
#include "graphics/button.h"
#include "graphics/ui.h"
#include "game/score.h"

constexpr float FALLING_TICK_DURATION = 0.01f;

class tetrisGame {
public:
    tetrisGame(tetrisEvent* event, tetrisUI* gameUI, tetrisScore* gameScore);
    ~tetrisGame();
    void Loop();
    void setTetrominoTexture(Texture2D texture);
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

    // Its texture
    std::vector<Texture2D> _textureMap;

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