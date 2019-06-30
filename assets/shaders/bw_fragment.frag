#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D screen;

void main()
{
   FragColor = texture2D(screen, TexCoord);
   FragColor.r = FragColor.g = FragColor.b = (FragColor.r + FragColor.g + FragColor.b)/3.0;
}
