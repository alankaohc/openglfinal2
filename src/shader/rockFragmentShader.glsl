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


layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gSpecular;

void main()
{

	//Normal Mapping

	vec3 V = normalize(fs_in.eyeDir);
	vec3 L = normalize(fs_in.lightDir);
	vec3 N = normalize(texture(NormalTexture, f_uv).rgb * 2.0 - vec3(1.0));
	vec3 R = reflect(-L, N);

	vec3 diffuse_albedo = texture(albedoTexture, f_uv).rgb;
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;

	vec3 specular_albedo = vec3(1.0);
	vec3 specular = max(pow(dot(R, V), 32.0), 0.0) * specular_albedo;
	

	// Normalize the incoming N, L and V vectors
	// Why we need to do normalization?
	
	vec4 texel = texture(albedoTexture, f_uv) ;
	if(texel.a < 0.5){
		discard ;
	 }
	/*
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);*/
	// Compute the diffuse and specular components for each fragment

	vec3 ambient = 0.2 * texel.xyz;
	vec3 color = diffuse + specular;
	color = pow(color, vec3(0.5));
	//vec3 diffuse = max(dot(N, L), 0.0) * texel.xyz;
	//vec3 specular = pow(max(dot(N, H), 0.0), shininess_power) * texel.xyz;
	// Write final color to the framebuffer

		
	gPosition = f_worldVertex;
    gNormal = normalize(f_worldNormal);
	gDiffuse = vec4(color , 1.0);
	gSpecular = vec4(1.0, 1.0, 1.0, 32.0);
	//fragColor = vec4(texel.xyz, 1.0);
	
}