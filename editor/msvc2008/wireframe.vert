#version 150 core

uniform mat4 in_MVPMatrix;
uniform vec2 in_ScaleW;
attribute vec4 in_Position01;
attribute vec4 in_Position02;

varying vec3 dist;

void main(void)
{
	float swizz = gl_Vertex.w;
	vec4 v4Position = gl_Vertex;
	v4Position.w = 1.0;
	
	gl_Position = in_MVPMatrix * v4Position;
	
	vec2 v2Point2D = gl_Position.xy / gl_Position.w;
	
	vec4 v4Position01 = in_MVPMatrix * in_Position01;
	vec2 v1 = in_ScaleW *(v4Position01.xy / v4Position01.w - v2Point2D);

	vec4 v4Position02 = in_MVPMatrix * in_Position02;
	vec2 v2 = in_ScaleW *(v4Position02.xy / v4Position02.w - v2Point2D);
	
	float fArea2D = abs(v1.x * v2.y - v1.y * v2.x);

	float h = fArea2D / length(v1 - v2);
	
	if(swizz < 0.1)
	{
		dist = vec3(h, 0, 0);
	}
	else if(swizz < 1.1)
	{
		dist = vec3(0, h, 0);
	}
	else
	{	
		dist = vec3(0, 0, h);
	}
		
	dist *= gl_Position.w;
}