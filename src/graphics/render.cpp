#include "graphics/render.h"
#include <iostream>

TetrisRenderer::TetrisRenderer()
    : _stage(gameStage::TITLE_SCREEN)
{
    //Init Shader
    ShaderInit();

    // Target textures
    _back = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _game = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _front = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    _texture_buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Loading base texture on the actual scene
    ChangeStage(_stage);
}

TetrisRenderer::~TetrisRenderer()
{
    UnloadShader(_Shader_blur);
    UnloadRenderTexture(_back);
    UnloadRenderTexture(_game);
    UnloadRenderTexture(_front);
    UnloadRenderTexture(_texture_buffer);
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

void TetrisRenderer::BeginDisplay(RendererLayer layer)
{
    // Before, we can detect if the layer needs to be updated or not.
    BeginTextureMode(*getRenderTextureFromLayer(layer));
}

void TetrisRenderer::EndDisplay()
{
    EndTextureMode();
}

void TetrisRenderer::ChangeStage(gameStage stage)
{
    _stage = stage;
    // Clearing all the textures that have the unloadState set to ONCE.
    _textureLoader.clearTextureBuffer();
    switch(stage)
    {
        case gameStage::TITLE_SCREEN:
            ClearRenderTexture(_front);
            _textureLoader.unload(textureId::TETROMINO_TILEMAP);
            _textureLoader.unload(textureId::TILESET_BLACK);
            _textureLoader.unload(textureId::TILESET_BLACK_BORDERLESS);
            _textureLoader.load(textureId::BUTTON_PLAY,unloadState::ONCE);
            _textureLoader.load(textureId::BUTTON_SETTINGS,unloadState::ONCE);
            _textureLoader.load(textureId::BUTTON_EXIT,unloadState::ONCE);
            _textureLoader.load(textureId::LOGO,unloadState::ONCE);
            break;
        case gameStage::GAME:
            ClearRenderTexture(_front);
            _textureLoader.load(textureId::TILESET_BLACK,unloadState::NEVER);
            _textureLoader.load(textureId::TILESET_BLACK_BORDERLESS,unloadState::NEVER);
            _textureLoader.load(textureId::TETROMINO_TILEMAP,unloadState::NEVER);
            break;
        case gameStage::MENU_SCREEN:
        case gameStage::GAME_OVER:
            _textureLoader.load(textureId::BUTTON_BASE,unloadState::ONCE);
            break;
    }    
}

void TetrisRenderer::Render(void)
{
    auto backPtr = &_back;
    auto frontPtr = &_front;

    switch (_stage) {
    case gameStage::MENU_SCREEN:
    case gameStage::GAME_OVER:
        // Shader optimisation : creating a texture buffer everytime the scene is called
        // The 'BeginShaderMode' in this exemple is only called once.
        if (!_shader_buffer[0]) {
            BeginTextureMode(_texture_buffer);
            BeginShaderMode(_Shader_blur);
            DrawTextureRec(_back.texture, { 0, 0, (float)_back.texture.width, (float)-_back.texture.height }, { 0, 0 }, WHITE);
            EndShaderMode();
            EndTextureMode();
            _shader_buffer[0] = true;
        }
        // We set the back pointer texture to the shader buffer created.
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
    DrawTextureRec(frontPtr->texture, { 0, 0, (float)frontPtr->texture.width, (float)-frontPtr->texture.height }, { 0, 0 }, WHITE);
    EndDrawing();
}

textureLoader& TetrisRenderer::GetTextureLoader() { return _textureLoader; }
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