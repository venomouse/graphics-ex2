#version 330

layout(location = 0) in vec4 position;
uniform mat4 translation;
uniform vec4 colorDirection;
uniform float colorScale;

out vec4 coloring_position;

void main()
{
	//coloring each vertex according to the angle between it position vector and a random direction
 	vec4 coloring_pos = (position - colorDirection)/colorScale;
 	//normalizing
 	coloring_position = vec4(coloring_pos.x, coloring_pos.y, coloring_pos.z, 1.0f);
    gl_Position = translation*position;
}
