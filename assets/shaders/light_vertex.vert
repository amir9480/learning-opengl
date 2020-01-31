#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec4 aBoneWeights;
layout (location = 5) in uint aBoneIds[4];
layout (location = 6) in mat4 instancedWorld;
layout (location = 10) in vec3 aLightPosition;
layout (location = 11) in vec3 aLightDirection;
layout (location = 12) in vec3 aLightColor;
layout (location = 13) in float aLightPower;
layout (location = 14) in float aLightCone;
layout (location = 15) in float aLightRadius;

const int LightTypeDirectional = 0;
const int LightTypePoint = 1;

out vec4 TexCoord;
out vec3 instanceLightColor;
out vec3 instanceLightDirection;
out vec3 instanceLightPosition;
out float instanceLightPower;
out float instanceLightCone;
out float instanceLightRadius;

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
		instanceLightCone = aLightCone;
		instanceLightPower = aLightPower;
	} else {
		gl_Position = vec4(aPos, 1.0) * MVP;
	}

	TexCoord = gl_Position;
}
