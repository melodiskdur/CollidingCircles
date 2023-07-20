#version 330 core
out vec4 FragColor;

#define MAX_SIZE 200

uniform vec2 positions[MAX_SIZE];
uniform float radii[MAX_SIZE];
uniform vec3 colors[MAX_SIZE];
uniform int num_circles;

uniform int shaderType;

float glow(float dist, float r, float I)
{
	return pow(r/dist, I);
}


vec4 glowShader()
{
	vec3 tcolor = vec3(0.0);
	for (int i = 0; i < num_circles; i++)
	{
		vec2 x = positions[i];
		float scaledRadius = radii[i];
		vec3 color = colors[i];
		float dist = length((gl_FragCoord.xy - x));
		float intensity = 0.80;
		if (dist > scaledRadius)
			intensity = 0.95;
		color = glow(dist, scaledRadius, intensity) * color;
		tcolor += (1.0 - exp(-color));
	}
	return vec4(tcolor, 1.0);
}


vec4 simpleShader()
{
	vec3 tcolor = vec3(0.0);
	for (int i = 0; i < num_circles; i++)
	{
		vec2 x = positions[i];
		float scaledRadius = radii[i];
		vec3 color = colors[i];
		float dist = length((gl_FragCoord.xy - x));
		if (dist < scaledRadius)
			tcolor = color;
	}
	return vec4(tcolor, 1.0);
}

void main()
{
	if (shaderType == 0)
		FragColor = simpleShader();
	else
		FragColor = glowShader();
}
