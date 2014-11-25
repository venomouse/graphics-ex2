#version 330

layout(location = 0) in vec4 position;
uniform mat4 translation;

void main()
{
    gl_Position = translation*position;
}
