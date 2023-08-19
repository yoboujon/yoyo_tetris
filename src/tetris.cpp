#include "tetris.h"
#include "button.h"
#include "controls.h"
#include "event.h"
#include "raylib.h"
#include "tetromino.h"
#include "ui.h"

#include <iostream>

tetrisGame::tetrisGame(tetrisEvent* event, tetrisUI* gameUI, tetromino::tetrominoNames name)
    : _eventPtr(event)
    , _gameControls(tetrisControls(gameUI->getElapsedTime()))
    , _gameUI(gameUI)
    , _fallingTick(0.0f)
    , _isGameOver(false)
    , _pauseMenu(false)
{
    _actualBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), gameUI->getTetrisStage(), &_gameControls);
    _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), gameUI->getTetrisStage(), &_gameControls);
}

tetrisGame::~tetrisGame()
{
    delete _actualBlock;
    delete _nextBlock;
}

void tetrisGame::Loop()
{
    if(!_pauseMenu)
        _fallingTick += GetFrameTime();

    // static display > next display > Falling block
    _staticBlocks.Display();
    _nextBlock->DisplayNext();

    // Check for pause
    if (IsKeyReleased(KEY_ESCAPE))
    {
        _pauseMenu = !_pauseMenu;
        _pauseMenu ? _gameUI->ChangeStage(gameStage::MENU_SCREEN) : _eventPtr->callEvent(eventType::MENU_CLOSED, eventUser::UI);
    }
    if(_eventPtr->OnEvent(eventType::MENU_CLOSED, eventUser::TETRIS))
        _pauseMenu = false;

    // If game over/ pause menu -> Only display the game
    if (_isGameOver || _pauseMenu) {
        if (_pauseMenu)
            _actualBlock->Display();
        return;
    }

    // If 'game' scene : check player input and collision.
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
        _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), _gameUI->getTetrisStage(), &_gameControls);
        if (_actualBlock->GameEnded(_staticBlocks.getRectangles())) {
            _isGameOver = true;
            _gameUI->ChangeStage(gameStage::GAME_OVER);
        }
    }
}

bool tetrisGame::gameFinished() { return _isGameOver; }
bool tetrisGame::pause() { return _pauseMenu; }