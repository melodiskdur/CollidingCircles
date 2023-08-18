#version 330 core

// This shader performs downsampling on a texture,
// as taken from Call Of Duty method, presented at ACM Siggraph 2014.
// This particular method was customly designed to eliminate
// "pulsating artifacts and temporal stability issues".
uniform sampler2D srcTexture;
uniform vec2 srcResolution;

in vec2 texCoord;
layout (location = 0) out vec4 downsample_out;

const int ARRAY_SIZE = 13;
const vec3 downsampleKernel[ARRAY_SIZE] = vec3[]
(
    vec3(-2.0, 2.0, 0.03125), vec3(0.0, 2.0, 0.0625), vec3(2.0, 2.0, 0.03125),
    vec3(-1.0, 1.0, 0.125), vec3(1.0, 1.0, 0.125),
    vec3(-2.0, 0.0, 0.0625), vec3(0.0, 0.0, 0.125), vec3(2.0, 0.0, 0.0625),
    vec3(-1.0, -1.0, 0.125), vec3(1.0, -1.0, 0.125),
    vec3(-2.0, -2.0, 0.03125), vec3(0.0, -2.0, 0.0625), vec3(2.0, -2.0, 0.03125)
);

void main()
{
    vec2 srcTexelSize = 1.0 / srcResolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    vec3 downsample = vec3(0.0);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        downsample += downsampleKernel[i].z * texture(srcTexture, vec2(
            texCoord.x + downsampleKernel[i].x * x,
            texCoord.y + downsampleKernel[i].y * y)).rgb;
    }
    downsample_out = vec4(downsample, 1.0);
}