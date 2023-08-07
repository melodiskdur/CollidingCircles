#version 330 core

layout (location = 0) in vec2 vPosition;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(vPosition, 0.0, 1.0);
    texCoord = 0.5 * (vPosition + 1);
}