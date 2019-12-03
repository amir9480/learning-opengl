#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D albedo;
uniform sampler2D normal;

void main()
{
   FragColor = texture2D(normal, TexCoord);
   //FragColor.g/= 3.0;
   //FragColor.b/= 3.0;
}
