
#version 450 core
layout(location=0) in vec3 v_vertex;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_uv;


uniform mat4 modelMat ;

void main() {

	vec4 worldVertex = modelMat * vec4(v_vertex, 1.0);
	worldVertex.w = 1.0;
	
	gl_Position = worldVertex;

}
