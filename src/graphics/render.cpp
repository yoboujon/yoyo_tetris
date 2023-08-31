#include "graphics/render.h"
#include <iostream>

tetrisRenderer::tetrisRenderer(tetrisEvent& event)
    : _eventPtr(event)
{

}

tetrisRenderer::~tetrisRenderer()
{

}

void tetrisRenderer::BeginDisplay(rendererLayer layer)
{
    // Starting the render texture of the incomming data
}

void tetrisRenderer::EndDisplay(rendererLayer layer)
{
    // Creating a texture for each layer depending on the actual stage
}


void tetrisRenderer::Render()
{
    _eventPtr.callEvent(eventType::NONE,eventUser::ANY);
    // Render the texture created
}