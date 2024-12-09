
#version 450 core
layout(location=0) in vec3 v_vertex;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_uv;
layout(location=3) in vec3 v_tangent;

uniform mat4 projMat ;
uniform mat4 viewMat ;
uniform mat4 modelMat ;
uniform vec4 v_worldPosOffset; 



void main() {

	vec4 worldVertex = vec4(v_vertex, 1.0) + v_worldPosOffset;
	worldVertex.w = 1.0;
	
	gl_Position = worldVertex;

}
