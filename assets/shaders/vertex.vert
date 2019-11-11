#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instancedWorld;

out vec3 Normal;
out vec2 TexCoord;

uniform bool instancing = false;
uniform mat4 MVP;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	if (instancing) {
		gl_Position = vec4(aPos, 1.0) * (instancedWorld * viewProjectionMatrix);
		Normal = aNormal * mat3x3(instancedWorld);
	} else {
		gl_Position = vec4(aPos, 1.0) * MVP;
		Normal = aNormal * mat3x3(worldMatrix);
	}
	TexCoord = aTexCoord;
}
