#version 330 core

const int LightTypeDirectional = 0;
const int LightTypePoint = 1;
const int LightTypeSpot = 2;

out vec4 FragColor;
  
in vec4 TexCoord;
in vec3 instanceLightColor;
in vec3 instanceLightDirection;
in vec3 instanceLightPosition;
in float instanceLightPower;
in float instanceLightCone;
in float instanceLightRadius;

vec2 screenCoord;

uniform bool instancing = false;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDirection = normalize(vec3(0.5, -0.5, 0.5));
uniform int lightType = LightTypeDirectional;
uniform vec3 lightPosition = vec3(0.0, 3.0, 0.0);
uniform float lightRadius =  250.0;
uniform float lightCone =  1.0;
uniform float lightPower = 1.0;
uniform vec3 camPos = vec3(0, 0, 0);
uniform float camNear;
uniform float camFar;
uniform mat4x4 MVP;
uniform mat4x4 viewProjection;
uniform mat4x4 view;
uniform mat4x4 projection;
uniform mat4x4 viewProjectionInv;
uniform mat4x4 viewInv;
uniform mat4x4 projectionInv;
uniform mat4x4 shadowVP[4];
uniform bool hasShadow = false;

uniform sampler2D final;
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D tangent;
uniform sampler2D depth;
uniform sampler2D theShadowMap[4];

vec2 poissonDisk[16] = vec2[](
   vec2(-0.613392, 0.617481),
   vec2(0.170019, -0.040254),
   vec2(-0.299417, 0.791925),
   vec2(0.645680, 0.493210),
   vec2(-0.651784, 0.717887),
   vec2(0.421003, 0.027070),
   vec2(-0.817194, -0.271096),
   vec2(-0.705374, -0.668203),
   vec2(0.977050, -0.108615),
   vec2(0.063326, 0.142369),
   vec2(0.203528, 0.214331),
   vec2(-0.667531, 0.326090),
   vec2(-0.098422, -0.295755),
   vec2(-0.885922, 0.215369),
   vec2(0.566637, 0.605213),
   vec2(0.039766, -0.396100)
);

struct LightInstanceData
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float power;
	float radius;
	float cone;
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
    return 2.0 * camNear * camFar / (camFar + camNear - z_n * (camFar - camNear));
}

void main()
{
	vec3 _lightColor = lightColor;
	vec3 _lightDirection = normalize(lightDirection);
	vec3 _lightPosition = lightPosition;
	float _lightRadius = lightRadius;
	float _lightCone = lightCone;
	float _lightPower = lightPower;
	if (instancing) {
		_lightColor = instanceLightColor;
		_lightPosition = instanceLightPosition;
		_lightDirection = normalize(instanceLightDirection);
		_lightRadius = instanceLightRadius;
		_lightCone = instanceLightCone;
		_lightPower = instanceLightPower;
	}
	
	screenCoord = (TexCoord.xy / TexCoord.w) * 0.5 + 0.5;
	vec4 albedoPixel = texture2D(albedo, screenCoord);
	vec3 normalVec = texture2D(normal, screenCoord).xyz;
	normalVec = normalize((normalVec * 2.0) - 1.0);
	float depthValue = texture2D(depth, screenCoord).r;
	vec3 worldPos = worldPosFromDepth(depthValue);
	vec3 camToWorld = worldPos - camPos;
	float camToWorldLength = length(worldPos - camPos);
	
	float visibility = 1;
	vec3 diffuse = albedoPixel.rgb;
	vec3 specular = vec3(0, 0, 0);


	if (lightType == LightTypeDirectional) {
		diffuse *= max(0.0, dot(normalVec, -_lightDirection)) * _lightColor * _lightPower;
		vec3 reflectDir = reflect(-_lightDirection, normalVec);
		specular = vec3(pow(max(0, dot(normalize(worldPos - camPos), reflectDir)), 64)) * albedoPixel.rgb * _lightColor * _lightPower;
		if (hasShadow) {
			for (int i = 0; i < 4; i++) {
				vec4 shadowSpacePos = (vec4(worldPos, 1) * shadowVP[i]);
				shadowSpacePos = vec4(shadowSpacePos.xyz / shadowSpacePos.w, 1);
				shadowSpacePos = shadowSpacePos * 0.5 + 0.5;
				float bias = 0.0001f + (i > 1 ? i * 0.001f : i*0.0001f);
				if (clamp(shadowSpacePos.x, 0.001, 0.999) == shadowSpacePos.x && clamp(shadowSpacePos.y, 0.001, 0.999) == shadowSpacePos.y) {
					//for (int j = -1; j <= 1; j++) {
					//	for (int k = -1; k <= 1; k++) {
					//		if (texture(theShadowMap[i], vec2(shadowSpacePos.x + (j* (1.0 / float(textureSize(theShadowMap[i], 0).y / 2))), shadowSpacePos.y + (k*(1.0 / float(textureSize(theShadowMap[i], 0).y))))).r + bias < shadowSpacePos.z) {
					//			visibility -= 0.0555;
					//		}
					//	}
					//}
					for (int j = 0; j < 8; j++) {
						float dot_product = dot(vec4(gl_FragCoord.xyy, j), vec4(12.9898,78.233,45.164,94.673));
						int index = int(16.0 * fract(sin(dot_product) * 53758.5453))%16;
						float theDepth = texture(theShadowMap[i], shadowSpacePos.xy + poissonDisk[index]/(700.0 + 200.0*i)).r;
						if (shadowSpacePos.z <= 1  && theDepth + bias < shadowSpacePos.z) {
							visibility -= 0.125;
						}
					}
					break;
				}
			}
		}
		//float l = texture2D(theShadowMap, TexCoord.xy *0.5 + 0.5).r;
		//l = (2.0 * 0.01) / (100.0 + 0.01 - l * (100.0 - 0.01));
		//FragColor = texture2D(theShadowMap, TexCoord.xy*0.5 + 0.5);
		//FragColor = vec4(l,0,0,1);
		//return;
	} else if (lightType == LightTypePoint) {
		vec3 lightToPixel = normalize(worldPos - _lightPosition);
		float lightToPixelDistance = distance(worldPos, _lightPosition);
		float att = clamp(1.0 - (lightToPixelDistance * lightToPixelDistance * lightToPixelDistance /(_lightRadius * _lightRadius)), 0.0, 1.0);
		att *= att;
		diffuse *= max(0.0, dot(normalVec, -lightToPixel)) * _lightColor * att * _lightPower;
		if (camToWorldLength < _lightRadius * 2) {
			vec3 reflectDir = reflect(-lightToPixel, normalVec);
			specular = clamp((clamp(3.0 * (1 - length(camToWorld) / (_lightRadius * 2)), 0, 1)) * vec3(pow(max(0, dot(normalize(camToWorld), reflectDir)), 64)) * albedoPixel.rgb * _lightColor * att * _lightPower, 0, 1);
		}
	} else if (lightType == LightTypeSpot) {
		vec3 lightToPixel = normalize(worldPos - _lightPosition);
		float lightToPixelDistance = distance(worldPos, _lightPosition);
		float att = clamp(1.0 - (lightToPixelDistance * lightToPixelDistance * lightToPixelDistance /(_lightRadius * _lightRadius)), 0.0, 1.0);
		float coneAtt = clamp(dot(lightToPixel, _lightDirection) / cos(_lightCone), 0, 1);

		att = (att * att) * pow(coneAtt, 16);
		
		diffuse *= max(0.0, dot(normalVec, -lightToPixel)) * _lightColor * att * _lightPower;
		if (camToWorldLength < _lightRadius * 2) {
			vec3 reflectDir = reflect(-lightToPixel, normalVec);
			specular = clamp((clamp(3.0 * (1 - length(camToWorld) / (_lightRadius * 10)), 0, 1)) * vec3(pow(max(0, dot(normalize(camToWorld), reflectDir)), 64)) * albedoPixel.rgb * _lightColor * att * _lightPower, 0, 1);
		}
	}


	FragColor = visibility * vec4(diffuse + specular, albedoPixel.a);
	
	//FragColor = vec4((normalVec + 0.5) / 2, 1);
}
