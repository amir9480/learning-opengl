#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instancedWorld;
layout (location = 7) in vec3 aLightPosition;
layout (location = 8) in vec3 aLightDirection;
layout (location = 9) in vec3 aLightColor;
layout (location = 10) in float aLightPower;
layout (location = 11) in float aLightRadius;

const int LightTypeDirectional = 0;
const int LightTypePoint = 1;

out vec4 TexCoord;
out vec3 instanceLightColor;
out vec3 instanceLightDirection;
out vec3 instanceLightPosition;
out float instanceLightRadius;
out float instanceLightPower;

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
		instanceLightColor = aLightColor;
		instanceLightDirection = aLightDirection;
		instanceLightPosition = aLightPosition;
		instanceLightRadius = aLightRadius;
		instanceLightPower = aLightPower;
	} else {
		gl_Position = vec4(aPos, 1.0) * MVP;
	}

	TexCoord = gl_Position;
}
