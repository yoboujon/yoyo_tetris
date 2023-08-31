#include "graphics/render.h"

tetrisRenderer::tetrisRenderer(tetrisEvent* event)
    : _eventPtr(event)
{

}

tetrisRenderer::~tetrisRenderer()
{

}

void tetrisRenderer::BeginDisplay(renderLayer layer)
{
    // Starting the render texture of the incomming data
}

void tetrisRenderer::EndDisplay(renderLayer layer)
{
    // Creating a texture for each layer depending on the actual stage
}


void tetrisRenderer::Render()
{
    // Render the texture created
}