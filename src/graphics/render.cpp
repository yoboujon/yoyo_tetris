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

    // Render Texture
    _rendererTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _texture_buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Changing the actual stage
    // And loading the correct textures
    ChangeStage(_stage);
    UpdateTexturesStage(_stage);
}

TetrisRenderer::~TetrisRenderer()
{
    UnloadShader(_Shader_blur);
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
    if(_eventHandler != nullptr)
        _eventHandler->sendEvent(this,EventType::CHANGING_STAGE, stage);
}

void TetrisRenderer::UpdateTexturesStage(gameStage stage)
{

    // Clearing all the textures that have the unloadState set to ONCE.
    _textureLoader.clearTextureBuffer();
    switch(stage)
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
            // Loading the shadered texture
            BeginTextureMode(_texture_buffer);
            BeginShaderMode(_Shader_blur);
            DrawTextureRec(_rendererTexture.texture, { 0, 0, (float)_rendererTexture.texture.width, (float)-_rendererTexture.texture.height }, { 0, 0 }, WHITE);
            EndShaderMode();
            EndTextureMode();
            break;
        case gameStage::SETTINGS:
            _textureLoader.load(textureId::TILESET_SETTINGS_WHITE, unloadState::ONCE);
            _textureLoader.load(textureId::BUTTON_RETURN, unloadState::ONCE);
            _textureLoader.load(textureId::TEXT_BOX, unloadState::ONCE);
            break;
    }
    _stage = stage;
    // When finishing loading the textures, inform the GameComponent
    if(_eventHandler != nullptr)
        _eventHandler->sendEvent(this, EventType::TEXTURES_LOADED);
}

void TetrisRenderer::RenderTexture(RendererLayer layer)
{
    switch (_stage) {
    case gameStage::MENU_SCREEN:
    case gameStage::GAME_OVER:
        if(layer == RendererLayer::GAME)
        {
            BeginBlendMode(BLEND_ALPHA);
            DrawTextureRec(_texture_buffer.texture, { 0, 0, (float)_texture_buffer.texture.width, (float)-_texture_buffer.texture.height }, { 0, 0 }, WHITE);
            DrawRectangleRec({0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, PAUSE_COLOR);
            EndBlendMode();
        }
        break;
    default:
        break;
    }
}

textureLoader& TetrisRenderer::GetTextureLoader() { return _textureLoader; }
Texture2D TetrisRenderer::GetTexture(textureId id) { return _textureLoader.getTexture(id); }
gameStage TetrisRenderer::GetStage() { return _stage; }