#version 400 core

uniform vec2 ScreenSize;
uniform mat4 ViewRotation;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec2 FragTexCoord;
out vec3 FragViewRay;

const float FOV = 45.0;
const float HFOV = FOV * 0.5;

void main()
{
    FragTexCoord = TexCoord;
    
    float AspectRatio = ScreenSize.x / ScreenSize.y;
    FragViewRay.x = AspectRatio * (TexCoord.x - 0.5);
    FragViewRay.y = TexCoord.y - 0.5;
    FragViewRay.z = 0.5 / tan(radians(HFOV));
    FragViewRay *= inverse(mat3(ViewRotation));
    
    gl_Position = vec4(Position, 1.0f);
}

