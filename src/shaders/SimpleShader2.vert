#version 330

layout(location = 0) in vec4 position;
uniform mat4 circTranslation;

void main()
{
    gl_Position = circTranslation*position;
}
