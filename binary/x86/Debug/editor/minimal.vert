#version 150 core

uniform mat4 in_MVPMatrix;
out vec4 pass_Color;

void main(void)
{
	gl_Position = in_MVPMatrix * gl_Vertex;
	pass_Color = vec4(1.0, 1.0, 1.0, 1.0);
}
