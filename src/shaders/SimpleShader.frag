#version 330

in vec4 coloring_position;

out vec4 outColor;

void main()
{
	outColor = coloring_position;
}
