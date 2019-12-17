#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D albedo;
uniform sampler2D final;

void main()
{
	FragColor = texture2D(final, TexCoord);
	if (texture2D(final, TexCoord).a <= 0.0001) {
		FragColor = vec4(0,140,250,1);
	}
}
