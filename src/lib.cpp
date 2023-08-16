#include "lib.h"
#include "raylib.h"

VectorMaths initArray(Vector2 a, Vector2 b)
{
    return VectorMaths{ a, b };
}

VectorMaths emptyArray()
{
    return VectorMaths{ NULL_VECTOR2, NULL_VECTOR2 };
}

void DrawTextureRatio(Texture2D texture, Rectangle source, float ratio, Vector2 position, Color tint)
{
    Rectangle screenRectangleRatio = {0,0, (source.width/ratio) , (source.height/ratio) };
    DrawTexturePro(texture, screenRectangleRatio, source, position, 0.0f, tint);
}