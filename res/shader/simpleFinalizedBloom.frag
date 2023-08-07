#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D sceneBuffer;
uniform sampler2D glowBuffer;

void main()
{
    vec3 sceneColor = texture(sceneBuffer, texCoord).rgb;
    vec3 glowColor = texture(glowBuffer, texCoord).rgb;
    FragColor = vec4(sceneColor + glowColor, 1.0);
}