#include "tetris.h"
#include "button.h"

#include <iostream>

gameTetris::gameTetris(Rectangle* gameRectangle, tetromino::tetrominoNames name, Vector2* mousePtr)
    : _actualBlock(new floatTetrisBlock(name, gameRectangle, &_gameControls) )
    , _actualName(name)
    , _gameRectangle(gameRectangle)
    , _gameOver(false)
{
    _buttonTexture = LoadTexture("res/base_button.png"); // Load button texture
    // Creating each button for the game over screen
    // TODO : Will be put in a separate object called "UI" That will be directly called by the main function
    _buttons.push_back( tetrisButton(mousePtr, &_buttonTexture, {0,50}, ButtonStyle::CENTERED) );
    _buttons.at(0).SetText("Restart");
    _buttons.push_back( tetrisButton(mousePtr, &_buttonTexture, {0,0}, ButtonStyle::CENTERED) );
    _buttons.at(1).SetText("Quit");
}

gameTetris::~gameTetris()
{
    UnloadTexture(_buttonTexture);
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
    for(auto& button : _buttons)
    {
        button.Update();
    }
}