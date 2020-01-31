#version 330 core

out vec4 FragColor;
  
in vec4 TexCoord;

uniform sampler2D albedo;
uniform sampler2D final;
uniform vec4 color;

void main()
{
	FragColor = (color * texture(albedo, TexCoord.xy)) + texture(final, TexCoord.xy);
}
