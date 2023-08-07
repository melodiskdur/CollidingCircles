#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D glowTexture;
uniform int horizontal;
uniform int mipLevel;
uniform float scale;

const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    //vec2 tex_offset = 1.0 / textureSize(glowTexture, mipLevel);
    vec2 tex_offset = 1.0 / textureSize(glowTexture, 0);
    //vec3 result = textureLod(glowTexture, texCoord, float(mipLevel)).rgb * weight[0];
    vec3 result = texture(glowTexture, texCoord).rgb * weight[0];
    if(horizontal == 1)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(glowTexture, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(glowTexture, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(glowTexture, texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(glowTexture, texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    float alpha = 1 / (mipLevel + 1);
    FragColor = vec4(result, alpha);
}