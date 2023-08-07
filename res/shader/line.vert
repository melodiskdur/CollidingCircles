#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(position.xy, 0.1, 1.0);
}