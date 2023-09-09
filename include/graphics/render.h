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

// ! The renderer should be rewritten in some ways...
// ! The optimisations i added can cause the textures layers to not be cleared completly.
// ! This could lead to weird artefacts and some ui elements not working because of buffering.

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
    void BeginDisplay(bool loading=false);
    void EndDisplay(bool loading=false);
    void ChangeStage(gameStage stage);
    void UpdateTexturesStage(gameStage stage);
    // Special function to render some shader based textures
    void RenderTexture(RendererLayer layer);
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
    void ThreadedChangeStage(gameStage stage);

    // Global Stage
    gameStage _stage;
    // Textures
    textureLoader _textureLoader;
    // Shaders
    Shader _Shader_blur;
    // Rendering texture
    RenderTexture2D _rendererTexture;
    RenderTexture2D _texture_buffer;

    // Variables
    bool _displaying;
};

#endif // HEADER_TETRIS_RENDER