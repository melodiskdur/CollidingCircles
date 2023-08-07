#version 330 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 cPosition;
layout (location = 2) in float cRadius;
layout (location = 3) in vec4 cColor;
layout (location = 4) in mat4 modelMatrix;

out vec3 fColor;
out float fRadius;
out vec2 fPosition;

uniform mat4 viewProjectionMatrix;

void main()
{
    vec2 screenSize = vec2(1280.0, 720.0);
    // vec4 pos = viewProjectionMatrix * vec4(vPosition * cRadius + cPosition, 0.0, 1.0);
    vec4 pos = viewProjectionMatrix * modelMatrix * vec4(vPosition, 0.0, 1.0);
    gl_Position = pos / pos.w;
    fPosition = vPosition;
    fColor = cColor.rgb;
    fRadius = cRadius;
}