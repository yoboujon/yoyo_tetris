#include "graphics/render.h"
#include <iostream>
#include <thread>
#include <chrono>

TetrisRenderer::TetrisRenderer()
    : _stage(gameStage::TITLE_SCREEN)
    , _displaying(false)
{
    //Init Shader
    ShaderInit();

    // Target textures
    _back = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _game = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _texture_buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _rendererTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Changing the actual stage
    // And loading the correct textures
    ChangeStage(_stage);
    UpdateTexturesStage();
}

TetrisRenderer::~TetrisRenderer()
{
    UnloadShader(_Shader_blur);
    UnloadRenderTexture(_back);
    UnloadRenderTexture(_game);
    UnloadRenderTexture(_front);
    UnloadRenderTexture(_texture_buffer);
    UnloadRenderTexture(_rendererTexture);
}

void TetrisRenderer::ShaderInit()
{
    // blur radius
    const int radius = 8;
    _Shader_blur = LoadShader(0, "res/shaders/blur.fs");
    SetShaderValue(_Shader_blur, GetShaderLocation(_Shader_blur, "radius"), &radius, SHADER_UNIFORM_INT);
    SetShaderValue(_Shader_blur, GetShaderLocation(_Shader_blur, "width"), &SCREEN_WIDTH, SHADER_UNIFORM_INT);
    SetShaderValue(_Shader_blur, GetShaderLocation(_Shader_blur, "height"), &SCREEN_HEIGHT, SHADER_UNIFORM_INT);
}

void TetrisRenderer::BeginDisplay(bool loading)
{
    // Before, we can detect if the layer needs to be updated or not.
    //BeginTextureMode(*getRenderTextureFromLayer(layer));
    if(!loading)
        BeginTextureMode(_rendererTexture);
}

void TetrisRenderer::EndDisplay(bool loading)
{
    if(!loading)
        EndTextureMode();
    BeginDrawing();
    DrawTextureRec(_rendererTexture.texture, { 0, 0, (float)_rendererTexture.texture.width, (float)-_rendererTexture.texture.height }, { 0, 0 }, WHITE);
    EndDrawing();
    // If loading state, we finish displaying the texture before ACTUALLY loading textures.
    if(loading)
        _eventHandler->sendEvent(this,EventType::END_DISPLAY_LOADING);
}

void TetrisRenderer::ChangeStage(gameStage stage)
{
    _stage = stage;
    if(_eventHandler != nullptr)
        _eventHandler->sendEvent(this,EventType::CHANGING_STAGE);
}

void TetrisRenderer::UpdateTexturesStage()
{

    // Clearing all the textures that have the unloadState set to ONCE.
    _textureLoader.clearTextureBuffer();
    switch(_stage)
    {
        case gameStage::TITLE_SCREEN:
            _textureLoader.unload(textureId::TETROMINO_TILEMAP);
            _textureLoader.unload(textureId::TILESET_BLACK);
            _textureLoader.unload(textureId::TILESET_BLACK_BORDERLESS);
            _textureLoader.load(textureId::BUTTON_PLAY,unloadState::ONCE);
            _textureLoader.load(textureId::BUTTON_SETTINGS,unloadState::ONCE);
            _textureLoader.load(textureId::BUTTON_EXIT,unloadState::ONCE);
            _textureLoader.load(textureId::LOGO,unloadState::ONCE);
            break;
        case gameStage::GAME:
            _textureLoader.load(textureId::TILESET_BLACK,unloadState::NEVER);
            _textureLoader.load(textureId::TILESET_BLACK_BORDERLESS,unloadState::NEVER);
            _textureLoader.load(textureId::TETROMINO_TILEMAP,unloadState::NEVER);
            break;
        case gameStage::MENU_SCREEN:
        case gameStage::GAME_OVER:
            _textureLoader.load(textureId::BUTTON_BASE,unloadState::ONCE);
            break;
        case gameStage::SETTINGS:
            _textureLoader.load(textureId::TILESET_SETTINGS_WHITE, unloadState::ONCE);
            _textureLoader.load(textureId::BUTTON_RETURN, unloadState::ONCE);
            _textureLoader.load(textureId::TEXT_BOX, unloadState::ONCE);
            break;
    }
    // When finishing loading the textures, inform the GameComponent
    if(_eventHandler != nullptr)
        _eventHandler->sendEvent(this, EventType::TEXTURES_LOADED);
}

void TetrisRenderer::Render(void)
{
    auto backPtr = &_back;
    auto gamePtr = &_game;
    auto frontPtr = &_front;

    switch (_stage) {
    case gameStage::MENU_SCREEN:
    case gameStage::GAME_OVER:
        // Shader optimisation : creating a texture buffer everytime the scene is called
        // The 'BeginShaderMode' in this exemple is only called once.
        if (!_shader_buffer[0]) {
            // Drawing the scene completly on a buffer
            auto render_buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
            BeginTextureMode(render_buffer);
            DrawTextureRec(_back.texture, { 0, 0, (float)_back.texture.width, (float)-_back.texture.height }, { 0, 0 }, WHITE);
            DrawTextureRec(_game.texture, { 0, 0, (float)_game.texture.width, (float)-_game.texture.height }, { 0, 0 }, WHITE);
            EndTextureMode();
            // Then applying the shader
            BeginTextureMode(_texture_buffer);
            BeginShaderMode(_Shader_blur);
            DrawTextureRec(render_buffer.texture, { 0, 0, (float)render_buffer.texture.width, (float)-render_buffer.texture.height }, { 0, 0 }, WHITE);
            EndShaderMode();
            EndTextureMode();
            // Setting the shader buffer to true -> not rerendering the shader + not showing the game
            _shader_buffer[0] = true;
        }
        // We set the GAME pointer texture to the shader buffer created.
        // Because it is the closest to the front which has to be displayed without any blur.
        backPtr = &_texture_buffer;
        break;
    default:
        for(auto& shader : _shader_buffer)
            shader = false;
        break;
    }

    // Begin to draw the scene
    BeginDrawing();
    // minus the height because the texture height is inverted in the RenderTexture2D.
    DrawTextureRec(backPtr->texture, { 0, 0, (float)backPtr->texture.width, (float)-backPtr->texture.height }, { 0, 0 }, WHITE);
    // Drawing the game isn't needed when the shader buffer is true.
    if(_shader_buffer[0] == false)
    {
        DrawTextureRec(gamePtr->texture, { 0, 0, (float)gamePtr->texture.width, (float)-gamePtr->texture.height }, { 0, 0 }, WHITE);
    }
    DrawTextureRec(frontPtr->texture, { 0, 0, (float)frontPtr->texture.width, (float)-frontPtr->texture.height }, { 0, 0 }, WHITE);
    EndDrawing();
    // Clearing the game texture
    ClearRenderTexture(_game);
}

textureLoader& TetrisRenderer::GetTextureLoader() { return _textureLoader; }
Texture2D TetrisRenderer::GetTexture(textureId id) { return _textureLoader.getTexture(id); }
gameStage TetrisRenderer::GetStage() { return _stage; }

RenderTexture2D* TetrisRenderer::getRenderTextureFromLayer(RendererLayer layer)
{
    RenderTexture2D* textureModePtr;
    switch(layer)
    {
        case RendererLayer::BACK:
            textureModePtr = &_back;
            break;
        case RendererLayer::GAME:
            textureModePtr = &_game;
            break;
        case RendererLayer::FRONT:
            textureModePtr = &_front;
            break;
    }
    return textureModePtr;
}