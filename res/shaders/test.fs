#version 330 core

uniform sampler2D texture0; // Texture input
uniform int screenWidth;    // Screen width uniform
uniform int screenHeight;   // Screen height uniform
uniform float elapsed;      // pointer to the actual time (must be updated with SetShaderValue)

in vec2 fragTexCoord;   // texture input coord between 0 and 1.0
in float iTime;
out vec4 fragColor;     // color output

vec3 palette (float t) {
    // [[0.618 0.338 1.148] [-0.542 -0.582 0.500] [-0.512 -0.382 1.000] [0.000 -0.582 0.667]]
    vec3 a = vec3(0.618, 0.338, 1.148);
    vec3 b = vec3(-0.542, -0.582, 0.500);
    vec3 c = vec3(-0.512, -0.382, 1.000);
    vec3 d = vec3(0.000, -0.582, 0.667);

    return a + b*cos( 6.28318*(c*t+d) );
}

void main()
{
    vec2 uv = (fragTexCoord- 0.5)*2.0;
    uv.x *= screenWidth/screenHeight;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    for(float i =0.0; i< 2.0; i++)
    {
        uv = fract(uv * 1.5) - 0.5;
        
        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + elapsed);
        
        d = sin(d*8.0 + elapsed)/8;
        d = abs(d);

        d = pow(0.01 / d, 2.0);
        finalColor += col * d;
    }

    fragColor = vec4(finalColor, 0.5); // Example color based on uv
}
