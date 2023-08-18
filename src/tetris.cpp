#include "tetris.h"
#include "button.h"
#include "controls.h"
#include "raylib.h"
#include "tetromino.h"

#include <iostream>

gameTetris::gameTetris(tetrisUI* gameUI, tetromino::tetrominoNames name)
    : _gameControls(controlsTetris(gameUI->getElapsedTime()))
    , _gameUI(gameUI)
    , _fallingTick(0.0f)
    , _isGameOver(false)
{
    _actualBlock = new floatTetrisBlock(tetromino::getRandomTetromino(), gameUI->getTetrisStage(), &_gameControls);
    _nextBlock = new floatTetrisBlock(tetromino::getRandomTetromino(), gameUI->getTetrisStage(), &_gameControls);
}

gameTetris::~gameTetris()
{
    delete _actualBlock;
    delete _nextBlock;
}

void gameTetris::Loop()
{
    _fallingTick += GetFrameTime();

    // static display > next display > Falling block
    _staticBlocks.Display();
    _nextBlock->DisplayNext();
    // If game over -> Only display the game
    if (_isGameOver)
        return;

    _staticBlocks.checkLine();
    const auto& collisionStatic = _staticBlocks.getRectangles();

    while (_fallingTick >= FALLING_TICK_DURATION) {
        _actualBlock->Fall(collisionStatic);
        _fallingTick -= FALLING_TICK_DURATION;
    }
    _actualBlock->Rotate(collisionStatic);
    _actualBlock->Move(collisionStatic);
    _actualBlock->Display();

    // Checking for collision
    if (_actualBlock->Placed()) {
        _staticBlocks.Add(*_actualBlock, _actualBlock->getColor());
        delete _actualBlock;
        _actualBlock = _nextBlock;
        _nextBlock = new floatTetrisBlock(tetromino::getRandomTetromino(), _gameUI->getTetrisStage(), &_gameControls);
        if (_actualBlock->GameEnded(_staticBlocks.getRectangles())) {
            _isGameOver = true;
        }
    }
}

bool gameTetris::gameFinished()
{
    return _isGameOver;
}

void gameTetris::DisplayTetrisNext()
{

}