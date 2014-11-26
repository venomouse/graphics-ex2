#version 330

layout(location = 0) in vec4 position;
uniform mat4 translation;

flat out vec4 coloring_position;

void main()
{
    gl_Position = translation*position;
    coloring_position = (vec4(1,1,1,1) + position)/2;
}
