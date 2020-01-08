#version 330 core

layout (location = 0) out vec4 GAlbedo;
layout (location = 1) out vec4 GNormal;
layout (location = 2) out vec4 GTangent;

in vec3 WorldPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;
in vec2 TexCoord;
in vec3 TangentCameraPos;
in vec3 TangentFragPos;

uniform mat4 MVP;
uniform vec3 cameraPosition;
uniform sampler2D diffuse;
uniform sampler2D normalTexture;
uniform bool hasNormal;
uniform sampler2D displacement;
uniform bool hasDisplacement;


vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float numLayers = mix(8.0, 64.0, abs(dot(vec3(0, 0, 1), viewDir)));
	float layerDepth = 1.0 / numLayers;
	vec2 result = texCoords;
    vec2 p = viewDir.xy * 0.05f;
	vec2 deltaTexcoords = p / numLayers;
	
    float currentLayerDepth = 0;
	float currentDepthMapValue = texture(displacement, texCoords).r;

	while (currentLayerDepth < currentDepthMapValue) {
		result -= deltaTexcoords;
		currentDepthMapValue = texture(displacement, texCoords).r;
		currentLayerDepth += layerDepth;
	}
    return result;   
}

void main()
{
	vec2 computedTexCoord = vec2(0, 0);
	float distanceToCamera = distance(cameraPosition, WorldPos);
	if (distanceToCamera < 20) {
		if (hasDisplacement) {
			computedTexCoord = parallaxMapping(TexCoord,  normalize(TangentCameraPos - TangentFragPos));
			if(computedTexCoord.x > 1.0 || computedTexCoord.y > 1.0 || computedTexCoord.x < 0.0 || computedTexCoord.y < 0.0)
				discard;
			if (distanceToCamera > 15) {
				computedTexCoord = mix(TexCoord, computedTexCoord, (20 - distanceToCamera) / 5.0);
			}
		} else {
			computedTexCoord = TexCoord;
		}
		if (hasNormal) {
			mat3 TBN = transpose(mat3(Tangent, BiTangent, Normal));
			vec3 bumpNormal = normalize(texture(normalTexture, computedTexCoord).rgb * 2.0 - 1.0);
			GNormal = vec4(normalize(bumpNormal * TBN)*0.5 + 0.5, 1);
			if (distanceToCamera > 15) {
				GNormal = mix(vec4(normalize(Normal)*0.5 + 0.5, 1.0), GNormal, (20 - distanceToCamera) / 5.0);
			}
		} else {
			GNormal = vec4(normalize(Normal)*0.5 + 0.5, 1.0);
		}
	} else {
		GNormal = vec4(normalize(Normal)*0.5 + 0.5, 1.0);
		computedTexCoord = TexCoord;
	}

	GAlbedo = vec4(texture2D(diffuse, computedTexCoord).rgb, 1.0);
	GTangent = vec4(normalize(Tangent)*0.5 + 0.5, 1.0);
}
