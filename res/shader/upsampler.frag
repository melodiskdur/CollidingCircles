#version 330 core

// This shader performs upsampling on a texture,
// as taken from Call Of Duty method, presented at ACM Siggraph 2014.
uniform sampler2D srcTexture;
uniform float filterRadius;

in vec2 texCoord;
layout (location = 0) out vec4 upsample_out;

const int ARRAY_SIZE = 9;
vec3 upsampleKernel[ARRAY_SIZE] = vec3[]
(
    vec3(-1.0, 1.0, 0.0625), vec3(0.0, 1.0, 0.125), vec3(1.0, 1.0, 0.0625),
    vec3(-1.0, 0.0, 0.125), vec3(0.0, 0.0, 0.25), vec3(1.0, 0.0, 0.125),
    vec3(-1.0, -1.0, 0.0625), vec3(0.0, -1.0, 0.125), vec3(1.0, -1.0, 0.0625)
);

void main()
{
    vec3 upsample = vec3(0.0);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        upsample += upsampleKernel[i].z * texture(srcTexture, vec2(
            texCoord.x + upsampleKernel[i].x * filterRadius,
            texCoord.y + upsampleKernel[i].y * filterRadius)).rgb;
    }
    upsample_out = vec4(upsample, 0.25);
}