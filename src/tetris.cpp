#include "tetris.h"
#include "button.h"
#include "controls.h"

#include <iostream>

gameTetris::gameTetris(tetrisUI* gameUI, tetromino::tetrominoNames name)
    : _gameControls(controlsTetris(gameUI->getElapsedTime())) 
    , _actualBlock(new floatTetrisBlock(name, gameUI->getTetrisStage(), &_gameControls))
    , _actualName(name)
    , _gameUI(gameUI)
    , _isGameOver(false)
{
    // Creating each button for the game over screen
    // TODO : Will be put in a separate object called "UI" That will be directly called by the main function
}

gameTetris::~gameTetris()
{
    delete _actualBlock;
}

void gameTetris::Loop()
{
    // Falling block + static display
    _staticBlocks.Display();
    // If game over -> Only display the game
    if (_isGameOver)
        return;
    _staticBlocks.checkLine();
    const auto& collisionStatic = _staticBlocks.getRectangles();
    _actualBlock->Fall(collisionStatic);
    _actualBlock->Rotate(collisionStatic);
    _actualBlock->Move(collisionStatic);
    _actualBlock->Display();

    // Checking for collision
    if (_actualBlock->Placed()) {
        // TODO : Random Block created
        //(_name == 6) ? name = 0 : name++;
        _staticBlocks.Add(*_actualBlock, _actualBlock->getColor());
        delete _actualBlock;
        _actualBlock = new floatTetrisBlock(_actualName, _gameUI->getTetrisStage(), &_gameControls);
        if (_actualBlock->GameEnded(_staticBlocks.getRectangles())) {
            _isGameOver = true;
        }
    }
}

bool gameTetris::gameFinished()
{
    return _isGameOver;
}
