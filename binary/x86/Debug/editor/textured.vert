#version 330 core

uniform mat4 in_MVPMatrix;
attribute vec4 in_v4Position;
attribute vec2 in_v2TextureCoord;
out vec2 UV;

void main(void)
{
	UV = in_v2TextureCoord;
	gl_Position = in_MVPMatrix * in_v4Position;
}
