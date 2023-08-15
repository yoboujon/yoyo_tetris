#include "tetris.h"

#include <iostream>

gameTetris::gameTetris(Rectangle* gameRectangle, tetromino::tetrominoNames name)
    : _actualBlock(new floatTetrisBlock(name, gameRectangle, &_gameControls) )
    , _actualName(name)
    , _gameRectangle(gameRectangle)
    , _gameOver(false)
{
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
    if(_gameOver) return;
    _staticBlocks.checkLine();
    const auto& collisionStatic = _staticBlocks.getRectangles();
    _actualBlock->Fall(collisionStatic);
    _actualBlock->Rotate(collisionStatic);
    _actualBlock->Move(collisionStatic);

    // Checking for collision
    if (_actualBlock->Placed()) {
        // TODO : Random Block created
        //(_name == 6) ? name = 0 : name++;
        _staticBlocks.Add(*_actualBlock, _actualBlock->getColor());
        delete _actualBlock;
        _actualBlock = new floatTetrisBlock(_actualName, _gameRectangle, &_gameControls);
        if (_actualBlock->GameEnded(_staticBlocks.getRectangles()))
        {
            _gameOver = true;
        }
    }
}

bool gameTetris::gameFinished()
{
    return _gameOver;
}

void gameTetris::GameOverScreen()
{
    //Const
    const int gameOvertext = MeasureText("Game Over", 30);

    //Ui
    DrawRectangleRec({0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, PAUSE_COLOR);
    DrawText("Game Over", (SCREEN_WIDTH-gameOvertext)/2, (SCREEN_HEIGHT/2)-100, 30, RAYWHITE);
    //GuiDrawButton(&button, buttonState);
}