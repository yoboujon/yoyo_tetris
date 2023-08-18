#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

// NOTE: Render size values should be passed from code
const float renderWidth = 760;
const float renderHeight = 540;

void main()
{
    vec2 uv = (fragTexCoord- 0.5)*2.0;
    uv.x *= renderWidth/renderHeight; //from <-1 to +1>

    finalColor = texture2D(texture0,fragTexCoord) + vec4(0.5,0,0,0);
}