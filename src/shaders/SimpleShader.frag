#version 330

flat in vec4 coloring_position;

uniform vec4 fillColor;

out vec4 outColor;

void main()
{
	outColor = coloring_position;
}
