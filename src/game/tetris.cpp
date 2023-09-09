#include "game/tetris.h"

#include <iostream>

tetrisGame::tetrisGame(float* _elapsedPtr)
    : _gameControls(tetrisControls(_elapsedPtr)), _staticBlocks(), _fallingTick(0.0f), _isGameOver(false), _pauseMenu(false), _start(false)
{
    // Reserving the vector to the maximum tetromino name count.
    _textureMap.reserve(static_cast<int>(tetromino::tetrominoNames::Count));
    _actualBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), TETRIS_STAGE, &_gameControls);
    _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), TETRIS_STAGE, &_gameControls);
}

tetrisGame::~tetrisGame()
{
    delete _actualBlock;
    delete _nextBlock;
    for(auto texture : _textureMap)
    {
        UnloadTexture(texture);
    }
}

// ! Maybe creating a copy opertation could be more effective but meh, i'm more sure with this approach
void tetrisGame::reset(float* _elapsedPtr)
{
    delete _actualBlock;
    delete _nextBlock;
    for(auto texture : _textureMap)
    {
        UnloadTexture(texture);
    }
    _gameControls = tetrisControls(_elapsedPtr);
    _staticBlocks = tetrisStaticBlocks();
    _fallingTick = 0.0f;
    _isGameOver = false;
    _pauseMenu = false;
    _actualBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), TETRIS_STAGE, &_gameControls);
    _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), TETRIS_STAGE, &_gameControls);
}

void tetrisGame::Loop()
{
    // While the game isn't started, don't loop.
    if(!_start)
        return;

    // Only display static blocks/next blocks when in game
    if (!_pauseMenu)
    {
        _fallingTick += GetFrameTime();
    }
    // static display > next display > Falling block
    _staticBlocks.Display();
    _nextBlock->DisplayNext();

    // Check for pause
    if (IsKeyReleased(KEY_ESCAPE))
    {
        _pauseMenu = !_pauseMenu;
        if (_pauseMenu)
            this->_eventHandler->sendEvent(this, OPEN_MENU);
        else
            this->_eventHandler->sendEvent(this, ESCAPE_PRESSED_CLOSE_MENU);
    }

    // If game over/ pause menu -> return
    if (_isGameOver || _pauseMenu)
    {
        if (_pauseMenu)
            _actualBlock->Display();
        return;
    }

    // If 'game' scene : check player input and collision.
    // checking any line that can be destroyed and gathering the staticblock rectangles
    _staticBlocks.checkLine();
    const auto &collisionStatic = _staticBlocks.getRectangles();

    // Player inputs are the first one to be checked
    _actualBlock->Move(collisionStatic);
    _actualBlock->Rotate(collisionStatic);

    // Falling and collision/display is done afterward
    while (_fallingTick >= FALLING_TICK_DURATION)
    {
        _actualBlock->Fall(collisionStatic);
        _fallingTick -= FALLING_TICK_DURATION;
    }
    _actualBlock->Display();

    // Checking for collision
    if (_actualBlock->Placed())
    {
        _staticBlocks.Add(*_actualBlock, _actualBlock->getName());
        delete _actualBlock;
        _actualBlock = _nextBlock;
        _nextBlock = new tetrisFloatBlock(tetromino::getRandomTetromino(), TETRIS_STAGE, &_gameControls);
        _nextBlock->setTextures(_textureMap);
        if (_actualBlock->GameEnded(_staticBlocks.getRectangles()))
        {
            _isGameOver = true;
            this->_eventHandler->sendEvent(this, GAME_OVER);
        }
    }
}

void tetrisGame::setTetrominoTexture(Texture2D texture)
{
    // Clearing first the _textureMap from the game in case there are remainings.
    _textureMap.clear();
    // Setting the texture on static block because each block is 1:1
    // So the texture mapping should work directly
    _staticBlocks.setTexture(texture);
    // For floating blocks, we are using rectangles that are not 1:1, so the texture offset
    // of the map could display other colors tetrominos.
    // Each individual texture is cropped into an image and pushed back as a texture.
    for (int textureOffset=0; textureOffset != static_cast<int>(tetromino::tetrominoNames::Count) ; textureOffset++)
    {
        auto croppedImage = LoadImageFromTexture(texture);
        ImageCrop(&croppedImage, {(static_cast<int>(textureOffset) * TEXTURE_TETROMINO_SIZE), 0.0f, TEXTURE_TETROMINO_SIZE, TEXTURE_TETROMINO_SIZE});
        _textureMap.push_back(LoadTextureFromImage(croppedImage));
    }
    // Loading these textures to the floating tetrominos.
    _actualBlock->setTextures(_textureMap);
    _nextBlock->setTextures(_textureMap);
    // TODO : Add a function to delete all these textures afterwards. (Memory leaks).
}

bool tetrisGame::gameFinished() { return _isGameOver; }
tetrisStaticBlocks* tetrisGame::getStaticBlock() { return &_staticBlocks; }
bool tetrisGame::getPause() { return _pauseMenu; }
void tetrisGame::setPause(bool state) { _pauseMenu = state; }
void tetrisGame::StartGame() { _start = true; }