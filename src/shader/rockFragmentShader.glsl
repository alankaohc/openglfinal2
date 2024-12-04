#version 450 core

in vec3 f_viewVertex ;
in vec3 f_viewNormal ;
in vec3 f_worldVertex ;
in vec3 f_worldNormal ;
in vec2 f_uv ;
in mat3 TBN ;



uniform sampler2D albedoTexture ;
uniform sampler2D NormalTexture ;


// Input from vertex shader
in VS_OUT
{
vec3 N;
vec3 L;
vec3 V;
vec3 eyeDir;
vec3 lightDir;
} fs_in;
// Material properties

uniform float shininess_power = 200.0;


layout (location = 0) out vec4 gSpecular;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec3 gPosition;

uniform bool u_NormalMappingEnabled;

//layout (location = 4) out vec3 gView;

void main()
{
	vec3 V = normalize(fs_in.eyeDir);
	vec3 N = normalize(texture(NormalTexture, f_uv).rgb * 2.0 - vec3(1.0));
	vec3 diffuse_albedo = texture(albedoTexture, f_uv).rgb;
		
	gPosition = f_worldVertex;
    gNormal = N;
	gDiffuse = vec4(diffuse_albedo , 1.0);
	gSpecular = vec4(1.0, 1.0, 1.0, 32.0);
	if (u_NormalMappingEnabled) {
	}
	else {
		gNormal = diffuse_albedo;
	}
	//gView = V;
	
}