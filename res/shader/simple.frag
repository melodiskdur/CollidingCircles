#version 330 core
out vec4 FragColor;

const int MAX_SIZE = 1000;

layout(std140) uniform radiiBuffer
{
    vec4 radii[MAX_SIZE];
};

layout(std140) uniform positionsBuffer
{
    vec4 positions[MAX_SIZE];
};

layout(std140) uniform colorsBuffer
{
    vec4 colors[MAX_SIZE];
};

//uniform vec2 positions[MAX_SIZE];
//uniform float radii[MAX_SIZE];
//uniform vec3 colors[MAX_SIZE];
uniform int num_circles;

uniform int shaderType;

float glow(float dist, float r, float I)
{
	return pow(r/dist, I);
}

vec4 glowShader_bufferTextureVersion()
{
	vec3 tcolor = vec3(0.0);
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (i >= num_circles) break;

		vec2 x = positions[i].xy;
		//float scaledRadius = radii[i].x;
		float scaledRadius = positions[i].z;
		vec3 color = colors[i].rgb;
		float dist = length((gl_FragCoord.xy - x));
		float smoothness = 0.01;
		float intensity = pow(smoothstep(0.0, 2.0 * scaledRadius, dist), smoothness);
		//if (dist > scaledRadius)
		//	intensity = 0.95;
		color = glow(dist, scaledRadius, intensity) * color;
		tcolor += (1.0 - exp(-color));
	}
	return vec4(tcolor, 1.0);
}
/*
vec4 glowShader()
{
	vec3 tcolor = vec3(0.0);
	for (int i = 0; i < num_circles; i++)
	{
		vec2 x = positions[i];
		float scaledRadius = radii[i];
		vec3 color = colors[i].rgb;
		float dist = length((gl_FragCoord.xy - x));
		float intensity = 0.80;
		if (dist > scaledRadius)
			intensity = 0.95;
		color = glow(dist, scaledRadius, intensity) * color;
		tcolor += (1.0 - exp(-color));
	}
	return vec4(tcolor, 1.0);
}
*/
/*
vec4 simpleShader()
{
	vec3 tcolor = vec3(0.0);
	for (int i = 0; i < num_circles; i++)
	{
		vec2 x = positions[i];
		float scaledRadius = radii[i];
		vec3 color = colors[i].rgb;
		float dist = length((gl_FragCoord.xy - x));
		if (dist < scaledRadius)
			tcolor = color;
	}
	return vec4(tcolor, 1.0);
}
*/

vec4 simpleShader_bufferTextureVersion()
{
	vec3 tcolor = vec3(0.0);

	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (i >= num_circles) break;

		vec2 x = positions[i].xy;
		// float scaledRadius = radii[i].x;
		float scaledRadius = positions[i].z;
		vec3 color = colors[i].rgb;
		float distSquared = dot(gl_FragCoord.xy - x, gl_FragCoord.xy - x);
		if (distSquared < scaledRadius * scaledRadius)
		{
			return vec4(color, 1.0);
		}

	}

	return vec4(tcolor, 1.0);
}

void main()
{
	if (shaderType == 0)
		FragColor = simpleShader_bufferTextureVersion();
	else
		FragColor = glowShader_bufferTextureVersion();
}
