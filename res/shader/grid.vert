#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
	vec4 worldPos = projectionMatrix * viewMatrix * modelMatrix * vec4(position.xy, 0.0, 1.0);
	// vec4 worldPos = vec4(position.xy, 0.0, 1.0);
	gl_Position = worldPos / worldPos.w;
}