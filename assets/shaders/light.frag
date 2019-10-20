#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;


uniform vec3 lightColor = vec3(0.95, 0.7, 0.4);
uniform vec3 lightDirection = normalize(vec3(0.5, -0.5, 0.5));
uniform vec3 camPos = vec3(0,0,0);
uniform mat4x4 viewProjection;
uniform mat4x4 view;
uniform mat4x4 projection;
uniform mat4x4 viewProjectionInv;
uniform mat4x4 viewInv;
uniform mat4x4 projectionInv;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D depth;

vec3 WorldPosFromDepth(float d) {
    float z = d * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(TexCoord * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = clipSpacePosition * projectionInv;

    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = viewSpacePosition * viewInv;

    return worldSpacePosition.xyz;
}

float realDepth()
{
	float z_b = texture2D(depth, TexCoord).x;
    float z_n = 2.0 * z_b - 1.0;
    return 2.0 * 0.01 * 1000.0 / (1000.0 + 0.01 - z_n * (1000.0 - 0.01));
}

void main()
{
	vec3 albedoPixel = texture2D(albedo, TexCoord).xyz;
	vec3 normalVec = texture2D(normal, TexCoord).xyz;
	normalVec = normalize((normalVec * 2.0) - 1.0);
	vec3 worldPos = WorldPosFromDepth(texture2D(depth, TexCoord).r);

	vec3 ambient = 0.2 * albedoPixel * lightColor;
	vec3 diffuse = max(0.0, dot(normalVec, lightDirection)) * albedoPixel * lightColor;
	vec3 reflectDir = reflect(-lightDirection, normalVec);
	vec3 specular = vec3(pow(max(0, dot(normalize(worldPos - camPos), reflectDir)), 256)) * albedoPixel * lightColor;


	FragColor = vec4(ambient + diffuse + specular, 1);
}
