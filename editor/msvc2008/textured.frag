#version 330 core

uniform sampler2D TextureMap;
uniform bool AlphaDiscard;
in vec2 UV;

void main(void)
{
	if(texture2D(TextureMap, UV).w < 0.2)
	{
		if(AlphaDiscard)
		{
			discard;
		}
		else
		{
			gl_FragColor = texture2D(TextureMap, UV);
		}
	}
	else
	{
		gl_FragColor = texture2D(TextureMap, UV);
	}
	//gl_FragColor = vec4(1.6, 1.6, 1.6, 1.0);
	//gl_FragColor = texture2D(TextureMap, UV);
}
