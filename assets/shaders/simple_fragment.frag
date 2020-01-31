#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D screen;
uniform int depth;

void main()
{
    if (depth == 1) {
        float d = (texture(screen, TexCoord).r);
        float dd = (2.0 * 0.01) / (100.0 + 0.01 - d * (100.0 - 0.01));
        dd = 1 - dd;
        FragColor = vec4(dd,dd,dd,1);
    } else {
        FragColor = texture(screen, TexCoord);
    }
}
