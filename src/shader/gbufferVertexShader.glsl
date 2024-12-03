#version 450 core
layout(location=0) in vec3 v_vertex;
layout(location=1) in vec3 v_normal ;
layout(location=2) in vec3 v_uv ;
layout(location=3) in vec4 v_worldPosOffset; // per-instance data fetched from SSBO
layout(location=4) in vec4 v_col0; 
layout(location=5) in vec4 v_col1; 
layout(location=6) in vec4 v_col2; 
layout(location=7) in vec4 v_col3; 

out vec3 f_viewVertex ;
out vec3 f_viewNormal ;
out vec3 f_worldVertex ;
out vec3 f_worldNormal ;
out vec3 f_uv ;

uniform mat4 projMat ;
uniform mat4 viewMat ;
uniform mat4 modelMat ;





void main() {
	mat4 rotMat;
	rotMat[0] = v_col0;
	rotMat[1] = v_col1;
	rotMat[2] = v_col2;
	rotMat[3] = v_col3;
	vec4 worldVertex = rotMat * modelMat * vec4(v_vertex, 1.0) + v_worldPosOffset ;
	// the vertex¡¦s w must be 1
	worldVertex.w = 1.0;

	

	vec4 worldNormal = modelMat * vec4(v_normal, 0.0) ;
	f_worldVertex = worldVertex.xyz ;
	f_worldNormal = worldNormal.xyz ;
	vec4 viewVertex = viewMat * worldVertex ;
	vec4 viewNormal = viewMat * worldNormal;
	f_viewVertex = viewVertex.xyz ;
	f_viewNormal = viewNormal.xyz ;
	f_uv = v_uv ;


	gl_Position = projMat * viewVertex ;
}