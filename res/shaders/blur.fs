#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform int radius;
uniform int width;
uniform int height;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 returnColor = vec4(0);
    float boxSize = pow((2*radius)+1,2);

    for(int xs = -radius; xs <= radius; xs++)
    {
        for(int ys = -radius; ys <= radius; ys++)
        {
            vec2 offsetVector = vec2(xs,ys)/vec2(width,height);
            returnColor += texture2D(texture0, fragTexCoord+offsetVector);
        }
    }
    
    finalColor = returnColor / boxSize;
}
