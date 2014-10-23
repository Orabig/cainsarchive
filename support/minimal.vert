#version 150 core

uniform mat4 in_MVPMatrix;
in vec3 in_Position;
in vec3 in_Color;
out vec3 pass_Color;

void main(void)
{
	gl_Position = in_MVPMatrix * vec4(in_Position, 1.0);
	pass_Color = in_Color;
}
