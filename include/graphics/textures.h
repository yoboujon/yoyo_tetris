#ifndef HEADER_TETRIS_TEXTURES
#define HEADER_TETRIS_TEXTURES

#include "lib.h"

#include <string>
#include <map>
#include <memory>
#include <vector>

constexpr float TEXTURE_TETROMINO_SIZE = 8.0f;
constexpr float TEXTURE_TETROMINO_RATIO = 20.0f/8.0f;

enum class textureId
{
    BUTTON_BASE,
    BUTTON_PLAY,
    BUTTON_SETTINGS,
    BUTTON_EXIT,
    TILESET_WHITE,
    TILESET_BLACK,
    TILESET_BLACK_BORDERLESS,
    TILESET_SETTINGS_WHITE,
    TILESET_SETTINGS_BLACK,
    LOGO,
    TETROMINO_TILEMAP,
    BUTTON_RETURN,
    TEXT_BOX,
    UP_DOWN,
};

enum class unloadState
{
    ONCE,  // Will be unloaded as soon as clearTextureBuffer() is called
    NEVER, // Only unloaded if the game quits or forceUnload() is called
    UNDEFINED
};

struct textureManager
{
    textureId id;
    Texture2D texture;
    unloadState state;
};

const std::map<textureId, std::string> texturePath =
    {
        {textureId::BUTTON_BASE, "res/button.png"},
        {textureId::BUTTON_PLAY, "res/button_play.png"},
        {textureId::BUTTON_EXIT, "res/button_exit.png"},
        {textureId::BUTTON_SETTINGS, "res/button_settings.png"},
        {textureId::TILESET_WHITE, "res/tileset_w.png"},
        {textureId::TILESET_BLACK, "res/tileset_b.png"},
        {textureId::TILESET_BLACK_BORDERLESS, "res/tileset_b_borderless.png"},
        {textureId::TILESET_SETTINGS_WHITE, "res/settings_w.png"},
        {textureId::TILESET_SETTINGS_BLACK, "res/settings_b.png"},
        {textureId::LOGO, "res/yoyotetris.png"},
        {textureId::TETROMINO_TILEMAP, "res/tetromino.png"},
        {textureId::BUTTON_RETURN, "res/button_return.png"},
        {textureId::TEXT_BOX, "res/text_box.png"},
        {textureId::UP_DOWN, "res/up_down.png"}};

class textureLoader
{
public:
    textureLoader();
    ~textureLoader();

    /**
     * @brief Loads a texture with a given unload state.
     * Will return a null pointer if already loaded.
     *
     * @param id    the texture you want to load
     * @param state the unloading state when calling clearTextureBuffer()
     * @return Texture2D* pointer to the texture. Null if not loaded.
     */
    Texture2D *load(textureId id, unloadState state);

    /**
     * @brief Unloads all the the textures with the ONCE state.
     */
    void clearTextureBuffer();

    /**
     * @brief Brut unload of a texture ID. Useful only for unloadState::NEVER
     * Or in certain cases if the texture is only useful one time.
     *
     * @param id        the texture you want to unload
     * @return true     unload successful
     * @return false    failed to unload (from raylib or if the texture was not laoded)
     */
    bool unload(textureId id);

    Texture2D getTexture(textureId id);
    unloadState getUnloadingState(textureId id);

private:
    std::vector<textureManager> _textureBuffer;
};

#endif // HEADER_TETRIS_TEXTURES
