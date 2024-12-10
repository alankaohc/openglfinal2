
#version 450 core
layout(location=0) in vec3 v_vertex;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_uv;
layout(location=3) in vec3 v_tangent;


out vec3 f_worldVertex;
out vec3 f_worldNormal;
out vec2 f_uv;
out vec3 f_tanEyeDir;
out vec3 f_tanLightDir;
out mat3 f_TBN;

uniform mat4 projMat ;
uniform mat4 viewMat ;
uniform mat4 modelMat ;
uniform vec4 v_worldPosOffset; 



// Position of light
uniform vec3 light_pos = vec3(0.4, 0.5, 0.8);


void main() {
	vec4 P = modelMat * vec4(v_vertex, 1.0);
	mat4 mv_matrix = viewMat * modelMat;
	vec3 T = normalize(mat3(mv_matrix) * v_tangent);
	vec3 N = normalize(mat3(mv_matrix) * v_normal);
	vec3 B = cross(N, T);
	f_TBN = mat3(T,B,N);
	vec3 L = light_pos - P.xyz;
	vec3 V = -P.xyz;
	f_tanLightDir = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));
	f_tanEyeDir = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));

	vec4 worldVertex = modelMat * vec4(v_vertex, 1.0) + v_worldPosOffset ;
	worldVertex.w = 1.0;
	vec4 worldNormal = modelMat * vec4(v_normal, 0.0) ;
	f_worldVertex = worldVertex.xyz ;
	f_worldNormal = worldNormal.xyz ;
	vec4 viewVertex = viewMat * worldVertex ;
	vec4 viewNormal = viewMat * worldNormal;

	f_uv = v_uv ;
	gl_Position = projMat * viewVertex ;
}
