#version 150 core

uniform vec3 in_Wireframe_Color;
uniform vec3 in_Fill_Color;

varying vec3 dist;

void main(void)
{
	vec3 dist_vec = dist * gl_FragCoord.w;

	float fDistance = min(dist_vec[0], min(dist_vec[1], dist_vec[2]));

	float fIntensity = exp2(-2.0 * fDistance * fDistance);
	gl_FragColor.xyz = fIntensity * in_Wireframe_Color + (1.0 - fIntensity) * in_Fill_Color;
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
};