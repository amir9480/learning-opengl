#version 330 core

layout (location = 0) out vec4 GAlbedoSpec;
layout (location = 1) out vec4 GNormal;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D diffuse;

void main()
{
   GAlbedoSpec = vec4(texture2D(diffuse, TexCoord).rgb, 1.0);
   GNormal = vec4(Normal/2.0 + 0.5, 1.0);
}
