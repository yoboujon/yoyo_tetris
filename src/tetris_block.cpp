#include "tetris_block.h"

#include <iostream>

using namespace tetromino;

/* ========================== */
/*     FLOAT Tetris Block     */
/* ========================== */

tetrisFloatBlock::tetrisFloatBlock()
    : tetrisFloatBlock(tetrominoNames::LightBlue_I, NULL, NULL, NULL)
{
}

tetrisFloatBlock::tetrisFloatBlock(tetromino::tetrominoNames name, Rectangle* tetrisStage, tetrisControls* gameControls, Texture2D* tetrominoTexture)
    : _name(name)
    , _color(getColorTetromino(name))
    , _position({ TETRIS_STAGE.x, TETRIS_STAGE.y })
    , _placed(false)
    , _rotation(tetromino::tetrisRotation::NONE)
    , _tetrisStage(tetrisStage)
    , _gameControls(gameControls)
    , _tetrominoTexture(tetrominoTexture)
{
    _area_object = 0;
    _object = constructReactangle(name, _position, &_area_object, tetrisRotation::NONE, true);
}

tetrisFloatBlock::~tetrisFloatBlock() { }

void tetrisFloatBlock::Fall(const std::vector<Rectangle>& tetrisBlock)
{
    int fallSpeed(1);
    if (_gameControls->IsKeyDown(KEY_DOWN))
        fallSpeed = FALL_SPEED;
    for (int i = 0; i < fallSpeed; i++) {
        auto newObject = moveY(1); // the speed
        const bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
        if (canBePlaced) {
            _placed = true;
        } else {
            _object = newObject;
            _position.y = _position.y + 1;
        }
        Display();
    }
}

void tetrisFloatBlock::Move(const std::vector<Rectangle>& tetrisBlock)
{
    const bool keyLeft = IsKeyDown(KEY_LEFT);
    const bool keyRight = IsKeyDown(KEY_RIGHT);
    int offset(0);

    // Key Input
    // Xiantas bebou XNOR
    if (!(keyLeft ^ keyRight)) {
        return;
    }
    if (_gameControls->TempoKey(keyLeft, KEY_TIMING)) {
        offset = -BLOCK_SIZE;
    } else if (_gameControls->TempoKey(keyRight, KEY_TIMING)) {
        offset = BLOCK_SIZE;
    }

    auto newObject = moveX(offset); // the speed
    const bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
    if (!canBePlaced) {
        _object = newObject;
        _position.x = _position.x + offset;
    }
}

void tetrisFloatBlock::Rotate(const std::vector<Rectangle>& tetrisBlock)
{
    if (IsKeyPressed(KEY_UP)) {
        _rotation = (_rotation == tetromino::tetrisRotation::COUNTER_CLOCKWISE ? tetromino::tetrisRotation::NONE : static_cast<tetromino::tetrisRotation>(static_cast<int>(_rotation) + 1));
        auto newObject = constructReactangle(_name, _position, NULL, _rotation);
        const bool canBePlaced = (checkCollisionWith(newObject, tetrisBlock) || !checkGameRectangle(newObject));
        if (!canBePlaced) {
            _object = newObject;
        }
    }
}

void tetrisFloatBlock::Display()
{
    const auto textureOffset = static_cast<int>(_name)*TEXTURE_TETROMINO_SIZE;
    // ! For now really unoptimised, cropping the gpu texture onto an image and making it back to the gpu.
    // ! Really intensive for nothing, a better approach could be done.
    auto croppedImage = LoadImageFromTexture(*_tetrominoTexture);
    ImageCrop(&croppedImage, { textureOffset, 0.0f, TEXTURE_TETROMINO_SIZE, TEXTURE_TETROMINO_SIZE });
    Texture2D tempTexture = LoadTextureFromImage(croppedImage);
    for (auto& recVec : _object) {
        DrawTextureRatio(tempTexture, {0.0f,0.0f}, recVec, TEXTURE_TETROMINO_RATIO, { 0.0f, 0.0f }, WHITE);
        //DrawRectangleRec(recVec, _color);
    }
}

void tetrisFloatBlock::DisplayNext()
{
    const auto textureOffset = static_cast<int>(_name)*TEXTURE_TETROMINO_SIZE;
    // ! For now really unoptimised, cropping the gpu texture onto an image and making it back to the gpu.
    // ! Really intensive for nothing, a better approach could be done.
    auto croppedImage = LoadImageFromTexture(*_tetrominoTexture);
    ImageCrop(&croppedImage, { textureOffset, 0.0f, TEXTURE_TETROMINO_SIZE, TEXTURE_TETROMINO_SIZE });
    Texture2D tempTexture = LoadTextureFromImage(croppedImage);

    const int offsetStart = (TETROMINO_MAP_RECT(_name, 0, 0).y > 0) ? BLOCK_SIZE : 0;
    const int actualWidth = getWidth(_name);
    const float nextX = NEXT_POSITION.x + ((25 * TILE_RATIO) / 5);
    const float nextY = NEXT_POSITION.y + 42;
    Rectangle baseRec;
    try {
        baseRec = _object.at(0);
    } catch (...) {
        // TODO : Casting a window with the error and the error code.
        std::cout << "ERROR: inside tetrisFloatBlock, object is of size 0. Contact the developper." << std::endl;
    }
    for (auto& recVec : _object) {
        // Substracting the base rectangle with its actual position to gather the rectangle at 0,0
        const auto offsetVector = Vector2Subtract({ recVec.x, recVec.y }, { baseRec.x, baseRec.y });
        // offsetX -> Casting it at the middle of MAX_WIDTH depending on its width. Adding the offset of the Rectangle and its position
        // offsetY -> Adding the offset of its position, the offset of the starting position (see tetrominoMap), and the base Rectangle.
        const float offsetX = nextX + ((static_cast<float>(MAX_WIDTH - actualWidth) / 2) * BLOCK_SIZE) + offsetVector.x;
        const float offsetY = nextY + offsetStart + offsetVector.y;
        DrawTextureRatio(tempTexture, {0.0f,0.0f}, { offsetX, offsetY, recVec.width, recVec.height }, TEXTURE_TETROMINO_RATIO, { 0.0f, 0.0f }, WHITE);
        //DrawRectangleRec({ offsetX, offsetY, recVec.width, recVec.height }, _color);
    }
}

bool tetrisFloatBlock::checkGameRectangle(const std::vector<Rectangle>& newRectangles)
{
    float area(0);
    for (auto& newRect : newRectangles) {
        auto rectCollide = GetCollisionRec(newRect, *_tetrisStage);
        area += rectCollide.height * rectCollide.width;
    }
    return (area == _area_object);
}

bool tetrisFloatBlock::checkCollisionWith(const std::vector<Rectangle>& newRectangles, const std::vector<Rectangle>& collideRectangles)
{
    for (auto& newRect : newRectangles) {
        for (auto& collideRect : collideRectangles) {
            if (CheckCollisionRecs(newRect, collideRect))
                return true;
        }
    }
    return false;
}

std::vector<Rectangle> tetrisFloatBlock::moveX(int x)
{
    std::vector<Rectangle> newRec(_object);
    for (auto& rect : newRec) {
        rect.x = rect.x + x;
    }
    return newRec;
}

std::vector<Rectangle> tetrisFloatBlock::moveY(int y)
{
    std::vector<Rectangle> newRec(_object);
    for (auto& rect : newRec) {
        rect.y = rect.y + y;
    }
    return newRec;
}

Rectangle* tetrisFloatBlock::getRectangle(int index) { return &(_object.at(index)); }
const std::vector<Rectangle>& tetrisFloatBlock::getRectangles() { return _object; }
bool tetrisFloatBlock::Placed() { return _placed; }
tetromino::tetrominoNames tetrisFloatBlock::getName() { return _name; }
Color tetrisFloatBlock::getColor() { return _color; }
bool tetrisFloatBlock::GameEnded(const std::vector<Rectangle>& tetrisBlock) { return checkCollisionWith(tetrisBlock, _object); }

/* ========================== */
/*    STATIC Tetris Blocks    */
/* ========================== */

tetrisStaticBlocks::tetrisStaticBlocks()
    : tetrisStaticBlocks(NULL, NULL) {};

tetrisStaticBlocks::tetrisStaticBlocks(tetrisEvent* eventPtr, Texture2D* tetrominoTexture)
    : _event(eventPtr)
    , _tetrominoTexture(tetrominoTexture)
{
}

tetrisStaticBlocks::~tetrisStaticBlocks() { }

void tetrisStaticBlocks::Add(tetrisFloatBlock& tetrisBlock, tetromino::tetrominoNames tetrisName)
{
    Vector2 increment, otherIncrement;
    float RectangleSize, OtherSize;
    for (auto recVec : tetrisBlock.getRectangles()) {
        if (recVec.width > recVec.height) {
            increment = { 1, 0 };
            otherIncrement = { 0, 1 };
            RectangleSize = recVec.width / BLOCK_SIZE;
            OtherSize = recVec.height / BLOCK_SIZE;
        } else {
            increment = { 0, 1 };
            otherIncrement = { 1, 0 };
            RectangleSize = recVec.height / BLOCK_SIZE;
            OtherSize = recVec.width / BLOCK_SIZE;
        }
        // Othersize being the other side size of the rectangle (width or height).
        // Rectangle size being the main focus. We push back into the _tetrisblock vector each individual square.
        for (float i = 0.0f; i < OtherSize; i++) {
            for (float j = 0.0f; j < RectangleSize; j++) {
                const auto yCoord = (recVec.y + (j * increment.y * BLOCK_SIZE) + (i * otherIncrement.y * BLOCK_SIZE));
                _tetrisBlocks.push_back({ (recVec.x + (j * increment.x * BLOCK_SIZE) + (i * otherIncrement.x * BLOCK_SIZE)),
                    yCoord,
                    BLOCK_SIZE, BLOCK_SIZE });
                _tetrisNames.push_back(tetrisName);
                if (_lineMap.find(yCoord) == _lineMap.end()) {
                    _lineMap[yCoord] = 1;
                } else {
                    _lineMap[yCoord] += 1;
                }
            }
        }
    }
}

void tetrisStaticBlocks::Display()
{
    for (size_t i = 0; i < _tetrisBlocks.size(); i++) {
        auto textureOffset = static_cast<int>(_tetrisNames.at(i))*TEXTURE_TETROMINO_SIZE;
        DrawTextureRatio(*_tetrominoTexture, { textureOffset, 0.0f }, _tetrisBlocks.at(i), TEXTURE_TETROMINO_RATIO, { 0.0f, 0.0f }, WHITE);
        //DrawRectangleRec(_tetrisBlocks.at(i), _tetrisNames.at(i));
    }
}

void tetrisStaticBlocks::checkLine()
{
    for (auto it = _lineMap.begin(); it != _lineMap.end();) {
        // Found a line
        if (it->second == HORIZONTAL_GRID_SIZE) {
            _event->callEvent(eventType::TETRIS_LINE_COMPLETED, eventUser::SCORE);
            // TODO : Play animation
            for (size_t i = 0; i < _tetrisBlocks.size();) {
                if (_tetrisBlocks.at(i).y == it->first) {
                    _tetrisBlocks.erase(_tetrisBlocks.begin() + i);
                    _tetrisNames.erase(_tetrisNames.begin() + i);
                    i = 0;
                } else
                    i++;
            }
            // For every blocks that are higher than the destroyed block, update its position to fall.
            // Reseting the linemap for the selected y coordonate
            for (auto& block : _tetrisBlocks) {
                if (block.y < it->first)
                    block.y = block.y + BLOCK_SIZE;
            }
            _lineMap = updateLineMap();
            it = _lineMap.begin();
        } else {
            it++;
        }
    }
}

std::map<float, int> tetrisStaticBlocks::updateLineMap()
{
    std::map<float, int> newMap;
    for (const auto& block : _tetrisBlocks) {
        if (newMap.find(block.y) == newMap.end()) {
            newMap[block.y] = 1;
        } else {
            newMap[block.y] += 1;
        }
    }
    return newMap;
}

const std::vector<Rectangle>& tetrisStaticBlocks::getRectangles() { return _tetrisBlocks; }