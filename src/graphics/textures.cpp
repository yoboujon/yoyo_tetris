#include "graphics/textures.h"
#include <iostream>

textureLoader::textureLoader()
{
    // Pre-reserving the _texturebuffer to the texturePath size so we don't reallocate every push.
    _textureBuffer.reserve(texturePath.size());
}

textureLoader::~textureLoader()
{
    // Unloading all the textures
    for (const auto &manager : _textureBuffer)
        UnloadTexture(manager.texture);
}

Texture2D *textureLoader::load(textureId id, unloadState state)
{
    // Checking if the texture is already loaded.
    for (const auto &manager : _textureBuffer)
    {
        if (manager.id == id)
        {
            std::cout << "texture id:" << (int)id << "already loaded!" << std::endl;
            return NULL;
        }
    }
    auto it = texturePath.find(id);
    // Checking if the texture id corresponds to the map
    if (it == texturePath.end())
    {
        std::cout << "texture id:" << (int)id << "doesnt exist in the map!" << std::endl;
        return NULL;
    }
    // Loading the texture and checking if file is found
    auto loadedTexture = LoadTexture(it->second.c_str());
    if (loadedTexture.id == 0)
    {
        std::cout << "texture id:" << (int)id << "could not be loaded." << std::endl;
        return NULL;
    }
    // Pushing back the successfully loaded texture
    _textureBuffer.push_back({id, loadedTexture, state});
    return &(_textureBuffer.back().texture);
}

void textureLoader::clearTextureBuffer()
{
    auto it = _textureBuffer.begin();
    while (it != _textureBuffer.end())
    {
        auto manager = *it;
        // If the unloading state is to once, unloading the texture and erasing the vector.
        if (manager.state == unloadState::ONCE)
        {
            UnloadTexture(manager.texture);
            it = _textureBuffer.erase(it);
        }
        else
            it++;
    }
}

bool textureLoader::unload(textureId id)
{
    return true;
}

Texture2D textureLoader::getTexture(textureId id)
{
    for (auto &manager : _textureBuffer)
    {
        if (manager.id == id)
        {
            return manager.texture;
        }
    }
    std::cout << "[getTexture] texture id:" << (int)id << "is not loaded." << std::endl;
    return { 0 };
}

unloadState textureLoader::getUnloadingState(textureId id)
{
    for (auto &manager : _textureBuffer)
    {
        if (manager.id == id)
            return manager.state;
    }
    return unloadState::UNDEFINED;
}