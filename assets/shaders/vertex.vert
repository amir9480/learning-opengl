#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in uint aBoneIds[4];
layout (location = 5) in vec4 aBoneWeights;
layout (location = 6) in mat4 instancedWorld;

out vec3 WorldPos;
out vec3 Normal;
out vec3 Tangent;
out vec3 BiTangent;
out vec2 TexCoord;
out vec3 TangentCameraPos;
out vec3 TangentFragPos;
out vec3 ToCamera;

uniform bool instancing = false;
uniform mat4 MVP;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPosition;
uniform bool hasAnim = false;
uniform mat4 bones[64];

void main()
{
	if (instancing) {
		WorldPos = (vec4(aPos, 1.0) * instancedWorld).xyz;
		gl_Position = vec4(WorldPos, 1.0) * (viewProjectionMatrix);
		Normal = normalize(aNormal * mat3x3(instancedWorld));
		Tangent = normalize(aTangent * mat3x3(instancedWorld));
	} else {
		if (hasAnim) {
			//mat4 boneTransform = mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
			mat4 boneTransform = bones[int(aBoneIds[0])] * aBoneWeights[0];
			boneTransform += bones[int(aBoneIds[1])] * aBoneWeights[1];
			boneTransform += bones[int(aBoneIds[2])] * aBoneWeights[2];
			boneTransform += bones[int(aBoneIds[3])] * aBoneWeights[3];
			WorldPos = (vec4(aPos, 1.0) * boneTransform * worldMatrix).xyz;
			gl_Position = vec4(aPos, 1.0) * boneTransform * MVP;
			Normal = normalize(aNormal * mat3x3(boneTransform) * mat3x3(worldMatrix));
			Tangent = normalize(aTangent * mat3x3(boneTransform) * mat3x3(worldMatrix));
		} else {
			WorldPos = (vec4(aPos, 1.0) * worldMatrix).xyz;
			gl_Position = vec4(aPos, 1.0) * MVP;
			Normal = normalize(aNormal * mat3x3(worldMatrix));
			Tangent = normalize(aTangent * mat3x3(worldMatrix));
		}
	}
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	BiTangent = normalize(cross(Normal, Tangent));
    mat3 TBN = transpose(mat3(Tangent, BiTangent, Normal));
    TangentCameraPos = cameraPosition;
    TangentFragPos = WorldPos;
	ToCamera = normalize(cameraPosition - WorldPos) * TBN;

	TexCoord = aTexCoord;
}
