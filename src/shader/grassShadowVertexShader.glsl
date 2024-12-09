#version 450 core

layout(location=0) in vec3 v_vertex; 
layout(location=1) in vec3 v_normal;
layout(location=2) in vec3 v_uv;
layout(location=3) in vec4 v_worldPosOffset; 
layout(location=4) in vec4 v_col0; 
layout(location=5) in vec4 v_col1; 
layout(location=6) in vec4 v_col2; 
layout(location=7) in vec4 v_col3;  



void main() {
	mat4 rotMat;
	rotMat[0] = v_col0;
	rotMat[1] = v_col1;
	rotMat[2] = v_col2;
	rotMat[3] = v_col3;
	vec4 worldVertex = rotMat * vec4(v_vertex, 1.0) + v_worldPosOffset ;
	worldVertex.w = 1.0;

	gl_Position = worldVertex;
}