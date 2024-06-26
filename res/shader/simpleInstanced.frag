#version 330 core

layout (location = 0) out vec3 FragColor;
layout (location = 1) out vec3 GlowColor;

in vec3 fColor;
in float fRadius;
in vec2 fPosition;

uniform float glowIntensity;
uniform float colorIntensity;

void main()
{
    float distanceSquared = dot(fPosition, fPosition);
    if (distanceSquared <= 0.25)
    {
        FragColor = fColor;

        float glowFactor = 1.0 - smoothstep(0.11, 0.25, distanceSquared);
        GlowColor = vec3(colorIntensity * fColor + glowIntensity * glowFactor);
    }
    else
        discard;
}