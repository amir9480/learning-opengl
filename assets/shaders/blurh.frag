#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D albedo;
uniform sampler2D final;
uniform vec2 resolution;

void main()
{
	
	FragColor = vec4(0,0,0,1);
	
    vec2 off1 = vec2(1.411764705882353) * vec2(1.5,0);
    vec2 off2 = vec2(3.2941176470588234) * vec2(1.5,0);
    vec2 off3 = vec2(5.176470588235294) * vec2(1.5,0);
    FragColor += texture2D(final, TexCoord) * 0.1964825501511404;
    FragColor += texture2D(final, TexCoord + (off1 / resolution)) * 0.2969069646728344;
    FragColor += texture2D(final, TexCoord - (off1 / resolution)) * 0.2969069646728344;
    FragColor += texture2D(final, TexCoord + (off2 / resolution)) * 0.09447039785044732;
    FragColor += texture2D(final, TexCoord - (off2 / resolution)) * 0.09447039785044732;
    FragColor += texture2D(final, TexCoord + (off3 / resolution)) * 0.010381362401148057;
    FragColor += texture2D(final, TexCoord - (off3 / resolution)) * 0.010381362401148057;
	FragColor.a = 1;
}
