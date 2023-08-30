#include "game/tetris.h"

#include <iostream>

tetrisGame::tetrisGame(tetrisEvent* event, tetrisUI* gameUI, tetrisScore* gameScore)
    : _eventPtr(event)
    , _gameControls(tetrisControls(gameUI->getElapsedTime()))
    , _staticBlocks(event, gameUI->getTetrominoTexture())
    , _gameUI(gameUI)
    , _gameScore(gameScore)
    , _fallingTick(0.0f)
    , _isGameOver(false)
    , _pauseMenu(false)
{
    _actualBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), gameUI->getTetrisStage(), &_gameControls, gameUI->getTetrominoTexture());
    _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), gameUI->getTetrisStage(), &_gameControls, gameUI->getTetrominoTexture());
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
    // checking any line that can be destroyed and gathering the staticblock rectangles 
    _staticBlocks.checkLine();
    const auto& collisionStatic = _staticBlocks.getRectangles();

    // Player inputs are the first one to be checked
    _actualBlock->Move(collisionStatic);
    _actualBlock->Rotate(collisionStatic);

    // Falling and collision/display is done afterward
    while (_fallingTick >= FALLING_TICK_DURATION) {
        _actualBlock->Fall(collisionStatic);
        _fallingTick -= FALLING_TICK_DURATION;
    }
    _actualBlock->Display();

    // Checking for collision
    if (_actualBlock->Placed()) {
        _staticBlocks.Add(*_actualBlock, _actualBlock->getName());
        delete _actualBlock;
        _actualBlock = _nextBlock;
        _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), _gameUI->getTetrisStage(), &_gameControls, _gameUI->getTetrominoTexture());
        if (_actualBlock->GameEnded(_staticBlocks.getRectangles())) {
            _isGameOver = true;
            _gameUI->ChangeStage(gameStage::GAME_OVER);
        }
    }

    // Checking for score
    _gameScore->updateScore();
}

bool tetrisGame::gameFinished() { return _isGameOver; }
bool tetrisGame::pause() { return _pauseMenu; }