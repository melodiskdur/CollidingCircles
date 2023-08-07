#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	vec4 worldPos = viewProjectionMatrix * modelMatrix * vec4(position.xy, 0.1, 1.0);
	// vec4 worldPos = vec4(position.xy, 0.0, 1.0);
	gl_Position = worldPos / worldPos.w;
}