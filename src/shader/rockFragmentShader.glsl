#version 450 core

in vec3 f_worldVertex;
in vec3 f_worldNormal;
in vec2 f_uv;
in vec3 f_tanEyeDir;
in vec3 f_tanLightDir;

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
	vec3 N = normalize(texture(NormalTexture, f_uv).rgb * 2.0 - vec3(1.0));
	vec3 diffuse_albedo = texture(albedoTexture, f_uv).rgb;
		
	gPosition = f_worldVertex;
	gDiffuse = vec4(diffuse_albedo , 1.0);
	gSpecular = vec4(1.0, 1.0, 1.0, 32.0);
	if (u_NormalMappingEnabled) {
		gNormal = mix(N, f_worldNormal, 0.5);
	}
	else {
		gNormal = f_worldNormal;
	}
}