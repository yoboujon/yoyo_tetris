#include "lib.h"

VectorMaths initArray(Vector2 a, Vector2 b)
{
    return VectorMaths{ a, b };
}

VectorMaths emptyArray()
{
    return VectorMaths{ NULL_VECTOR2, NULL_VECTOR2 };
}

void DrawTextureRatio(Texture2D texture, Vector2 source, Rectangle destination, float ratio, Vector2 position, Color tint)
{
    Rectangle sourceRatio = {source.x, source.y, (destination.width/ratio) , (destination.height/ratio) };
    DrawTexturePro(texture, sourceRatio, destination, position, 0.0f, tint);
}

bool checkCollisionPointRecArray(Vector2 point, Rectangle* rectArray, int sizeArray)
{
    bool returnCollision(false);
    for(int i=0;i<sizeArray;i++)
    {
        returnCollision |= CheckCollisionPointRec(point,rectArray[i]);
    }
    return returnCollision;
}