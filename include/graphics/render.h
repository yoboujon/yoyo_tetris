#ifndef HEADER_TETRIS_RENDER
#define HEADER_TETRIS_RENDER

#include "lib.h"
#include "event/component.h"

#include "game/tetris.h"
#include "graphics/ui.h"
#include "graphics/textures.h"
#include "graphics/shaders.h"


// All the private elements inside the tetris renderer will be rendered one time.
// Each UI/Game elements will be rendered every frame.

class TetrisRenderer : public BaseComponent
{
public:
    // Singleton
    static TetrisRenderer& GetInstance()
    {
        static TetrisRenderer instance;
        return instance;
    }

    void ShaderInit();  // ! Temporary -> Will be moved to shader.cpp
    void BeginDisplay(RendererLayer layer);
    void EndDisplay();
    void ChangeStage(gameStage stage);
    void Render();
    textureLoader& GetTextureLoader();
    Texture2D GetTexture(textureId id);
    gameStage GetStage();
private:
    // Singleton
    TetrisRenderer();
    ~TetrisRenderer();
    TetrisRenderer(const TetrisRenderer&) = delete;
    TetrisRenderer& operator=(const TetrisRenderer&) = delete;
    // Private Functions
    RenderTexture2D* getRenderTextureFromLayer(RendererLayer layer);

    // Global Stage
    gameStage _stage;
    // Textures
    textureLoader _textureLoader;
    // Shaders
    Shader _Shader_blur;
    std::array<bool,2> _shader_buffer;
    RenderTexture2D _texture_buffer;
    // Target textures
    RenderTexture2D _back;
    RenderTexture2D _game;
    RenderTexture2D _front;
};

#endif // HEADER_TETRIS_RENDER