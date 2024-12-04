#version 450 core

in vec3 f_viewVertex ;
in vec3 f_viewNormal ;
in vec3 f_worldVertex ;
in vec3 f_worldNormal ;
in vec2 f_uv ;

uniform sampler2D albedoTexture ;

//layout (location = 0) out vec4 fragColor ;

// Input from vertex shader
in VS_OUT
{
vec3 N;
vec3 L;
vec3 V;
} fs_in;
// Material properties
uniform vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
uniform vec3 specular_albedo = vec3(0.7);
uniform float shininess_power = 200.0;

layout (location = 0) out vec4 gSpecular;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec3 gPosition;
//layout (location = 4) out vec3 gView;

void main()
{

	// Normalize the incoming N, L and V vectors
	// Why we need to do normalization?

	vec4 texel = texture(albedoTexture, f_uv) ;
	if(texel.a < 0.5){
		discard ;
	 }
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);
	// Compute the diffuse and specular components for each fragment
	vec3 diffuse = max(dot(N, L), 0.0) * texel.xyz;
	vec3 specular = pow(max(dot(N, H), 0.0), shininess_power) * texel.xyz;
	// Write final color to the framebuffer
	//fragColor = vec4(diffuse + specular, 1.0);
	gPosition = f_worldVertex;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(f_worldNormal);
    // and the diffuse per-fragment color
	gDiffuse = vec4(texel.xyz, 1.0);
	//fragColor = vec4(1.0,0.0,0.0,1.0);
	gSpecular = vec4(1.0, 1.0, 1.0, 32.0);
	//gView = V; 
}