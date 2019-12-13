#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in mat4 instancedWorld;

out vec3 WorldPos;
out vec3 Normal;
out vec3 Tangent;
out vec3 BiTangent;
out vec2 TexCoord;
out vec3 TangentCameraPos;
out vec3 TangentFragPos;

uniform bool instancing = false;
uniform mat4 MVP;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;

void main()
{
	if (instancing) {
		WorldPos = (vec4(aPos, 1.0) * instancedWorld).xyz;
		gl_Position = vec4(WorldPos, 1.0) * (viewProjectionMatrix);
		Normal = normalize(aNormal * mat3x3(instancedWorld));
		Tangent = normalize(aTangent * mat3x3(instancedWorld));
	} else {
		WorldPos = (vec4(aPos, 1.0) * worldMatrix).xyz;
		gl_Position = vec4(aPos, 1.0) * MVP;
		Normal = normalize(aNormal * mat3x3(worldMatrix));
		Tangent = normalize(aTangent * mat3x3(worldMatrix));
	}
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	BiTangent = normalize(cross(Tangent, Normal));
    mat3 TBN = transpose(mat3(Tangent, BiTangent, Normal));
    TangentCameraPos  = TBN * cameraPosition;
    TangentFragPos  = TBN * WorldPos;

	TexCoord = aTexCoord;
}
