#version 450 core

in vec3 f_worldVertex;
in vec3 f_worldNormal;
in vec2 f_uv;
in vec3 f_tanEyeDir;
in vec3 f_tanLightDir;
in mat3 f_TBN;

uniform sampler2D albedoTexture ;
uniform sampler2D NormalTexture ;

uniform float shininess_power = 200.0;

layout (location = 0) out vec4 gSpecular;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec3 gPosition;

uniform bool u_NormalMappingEnabled;

void main()
{
	vec3 V = normalize(f_tanEyeDir);
	vec3 N = texture(NormalTexture, f_uv).rgb;
	N = N * 2.0 - 1.0;
	vec3 M = normalize(N);
	N = normalize(f_TBN * N);
	vec3 diffuse_albedo = texture(albedoTexture, f_uv).rgb;
		
	gPosition = f_worldVertex;
	gDiffuse = vec4(diffuse_albedo , 1.0);
	gSpecular = vec4(1.0, 1.0, 1.0, 32.0);
	if (u_NormalMappingEnabled) {
		gNormal = N;
	}
	else {
		gNormal = f_worldNormal;
	}
}