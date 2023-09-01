#ifndef HEADER_TETRIS_RENDER
#define HEADER_TETRIS_RENDER

#include "lib.h"

#include "game/tetris.h"
#include "graphics/ui.h"
#include "graphics/textures.h"
#include "graphics/shaders.h"


// All the private elements inside the tetris renderer will be rendered one time.
// Each UI/Game elements will be rendered every frame.

enum class rendererLayer {
    BACK,
    FRONT
};

class tetrisRenderer
{
public:
    tetrisRenderer();
    ~tetrisRenderer();
    void BeginDisplay(rendererLayer layer);
    void EndDisplay(rendererLayer layer);
    void Render();
private:

    // Textures
    textureLoader _textureLoader;

    // Shaders
    Shader _Shader_blur;
    std::array<bool,2> _shader_buffer;
    RenderTexture2D _texture_buffer;

    // Target textures
    RenderTexture2D _back;
    RenderTexture2D _front;
};

#endif // HEADER_TETRIS_RENDER