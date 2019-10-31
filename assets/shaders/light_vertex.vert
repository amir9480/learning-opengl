#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec4 TexCoord;

uniform mat4 MVP;

void main()
{
    gl_Position = vec4(aPos, 1.0) * MVP;
	TexCoord = gl_Position;
}
