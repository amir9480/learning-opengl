#version 330 core

const int LightTypeDirectional = 0;
const int LightTypePoint = 1;

out vec4 FragColor;
  
in vec4 TexCoord;
in vec3 instanceLightColor;
in vec3 instanceLightDirection;
in vec3 instanceLightPosition;
in float instanceLightRadius;
in float instanceLightPower;

vec2 screenCoord;

uniform bool instancing = false;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDirection = normalize(vec3(0.5, -0.5, 0.5));
uniform int lightType = LightTypeDirectional;
uniform vec3 lightPosition = vec3(0.0, 3.0, 0.0);
uniform float lightRadius =  250.0;
uniform float lightPower = 1.0;
uniform vec3 camPos = vec3(0, 0, 0);
uniform mat4x4 MVP;
uniform mat4x4 viewProjection;
uniform mat4x4 view;
uniform mat4x4 projection;
uniform mat4x4 viewProjectionInv;
uniform mat4x4 viewInv;
uniform mat4x4 projectionInv;

uniform sampler2D final;
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D depth;

struct LightInstanceData
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float power;
	float radius;
};

vec3 worldPosFromDepth(float d) {
    float z = d * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(screenCoord * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = clipSpacePosition * projectionInv;

    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = viewSpacePosition * viewInv;

    return worldSpacePosition.xyz;
}

float realDepth()
{
	float z_b = texture2D(depth, screenCoord).x;
    float z_n = 2.0 * z_b - 1.0;
    return 2.0 * 0.01 * 1000.0 / (1000.0 + 0.01 - z_n * (1000.0 - 0.01));
}

void main()
{
	vec3 _lightColor = lightColor;
	vec3 _lightDirection = lightDirection;
	vec3 _lightPosition = lightPosition;
	float _lightRadius = lightRadius;
	float _lightPower = lightPower;
	if (instancing) {
		_lightColor = instanceLightColor;
		_lightPosition = instanceLightPosition;
		_lightDirection = instanceLightDirection;
		_lightRadius = instanceLightRadius;
		_lightPower = instanceLightPower;
	}
	
	screenCoord = (TexCoord.xy / TexCoord.w) * 0.5 + 0.5;
	vec3 albedoPixel = texture2D(albedo, screenCoord).xyz;
	vec3 normalVec = texture2D(normal, screenCoord).xyz;
	normalVec = normalize((normalVec * 2.0) - 1.0);
	float depthValue = texture2D(depth, screenCoord).r;
	vec3 worldPos = worldPosFromDepth(depthValue);
	vec3 camToWorld = worldPos - camPos;
	float camToWorldLength = length(worldPos - camPos);

	vec3 diffuse = albedoPixel.rgb;
	vec3 ambient = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);


	if (lightType == LightTypeDirectional) {
		ambient = 0.0 * albedoPixel * lightColor;
		diffuse *= max(0.0, dot(normalVec, -lightDirection)) * lightColor * lightPower;
		vec3 reflectDir = reflect(-lightDirection, normalVec);
		specular = vec3(pow(max(0, dot(normalize(worldPos - camPos), reflectDir)), 64)) * albedoPixel * lightColor * lightPower;
	} else if (lightType == LightTypePoint) {
		vec3 lightToPixel = normalize(worldPos - _lightPosition);
		float lightToPixelDistance = distance(worldPos, _lightPosition);
		float att = clamp(1.0 - lightToPixelDistance * lightToPixelDistance /(_lightRadius * _lightRadius), 0.0, 1.0);
		att *= att;
		diffuse *= max(0.0, dot(normalVec, -lightToPixel)) * _lightColor * att * _lightPower;
		if (camToWorldLength < _lightRadius * 2) {
			vec3 reflectDir = reflect(-lightToPixel, normalVec);
			specular = clamp((clamp(3.0 * (1 - length(camToWorld) / (_lightRadius * 2)), 0, 1)) * vec3(pow(max(0, dot(normalize(camToWorld), reflectDir)), 64)) * albedoPixel * _lightColor * att * _lightPower, 0, 1);
		}
	}


	FragColor = vec4(ambient + diffuse + specular, 1);
	
	//FragColor = vec4((normalVec + 0.5) / 2, 1);
}
