#version 430 core

layout (local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

struct DrawCommand{
	uint count ;
	uint instanceCount ;
	uint firstIndex ;
	uint baseVertex ;
	uint baseInstance ;
};

struct InstanceProperties{
	vec4 position ;
};
struct Colume{
	vec4 vector ;
};
layout (std430, binding=3) buffer DrawCommandsBlock{
	DrawCommand commands[] ;
};
layout (std430, binding=1) buffer InstanceData{
	InstanceProperties rawInstanceProps[] ;
};
layout (std430, binding=2) buffer CurrValidInstanceData{
	InstanceProperties currValidInstanceProps[] ;
};

layout (std430, binding=4) buffer RawColumeData0{
	Colume rawRotCol0[] ;
};
layout (std430, binding=5) buffer ValidColumeData0{
	Colume validRotCol0[] ;
};

layout (std430, binding=6) buffer RawColumeData1{
	Colume rawRotCol1[] ;
};
layout (std430, binding=7) buffer ValidColumeData1{
	Colume validRotCol1[] ;
};
layout (std430, binding=8) buffer RawColumeData2{
	Colume rawRotCol2[] ;
};
layout (std430, binding=9) buffer ValidColumeData2{
	Colume validRotCol2[] ;
};
layout (std430, binding=10) buffer RawColumeData3{
	Colume rawRotCol3[] ;
};
layout (std430, binding=11) buffer ValidColumeData3{
	Colume validRotCol3[] ;
};


uniform int numMaxInstance;
uniform mat4 viewProjMat;


void main() {
	const uint idx = gl_GlobalInvocationID.x;
	vec4 center;
	float radius;
	int con_n;
	if(idx >= numMaxInstance){ return; }

	if (idx>=0 && idx < commands[1].baseInstance) {
		center = vec4(0.0, 1.76, 0.0, 1.0);
		radius = 2.6;
		con_n = 0;
	}
	else if (idx>=commands[1].baseInstance && idx<commands[2].baseInstance) {
		center = vec4(0.0, 4.57, 0.0, 1.0);
		radius = 10.2;
		con_n = 1;
	}
	else if (idx>=commands[2].baseInstance && idx<commands[3].baseInstance) {
		center = vec4(0.0, 4.57, 0.0, 1.0);
		radius = 8.5;
		con_n = 2;
	}
	else if (idx>=commands[3].baseInstance && idx<commands[4].baseInstance) {
		center = vec4(0.0, 2.55, 0.0, 1.0);
		radius = 3.4;
		con_n = 3;
	}
	else if (idx>=commands[4].baseInstance) {
		center = vec4(0.0, 0.66, 0.0, 1.0);
		radius = 1.4;
		con_n = 4;
	}
	center.xyz = center.xyz + rawInstanceProps[idx].position.xyz;
	vec4 v_left = center + vec4(-radius, 0, 0, 1);
	vec4 v_right = center + vec4(radius, 0, 0, 1);
	vec4 v_top = center + vec4(0, radius, 0, 1);
	vec4 v_bottom = center + vec4(0, -radius, 0, 1);
	vec4 v_near = center + vec4(0, 0, radius, 1);
	vec4 v_far = center + vec4(0, 0, -radius, 1);
	vec4 array[6];
	array[0] = v_left;
	array[1] = v_right;
	array[2] = v_top;
	array[3] = v_bottom;
	array[4] = v_near;
	array[5] = v_far;
	bool frustumCulled = true;
	for (int i=0; i<6; i++) {
		vec4 clipSpaceV = viewProjMat * vec4(array[i].xyz, 1.0);
		clipSpaceV = clipSpaceV / clipSpaceV.w;
		bool isInside = (clipSpaceV.x > -1.0) && (clipSpaceV.x < 1.0) && (clipSpaceV.y > -1.0) &&
		(clipSpaceV.y < 1.0) && (clipSpaceV.z > -1.0) && (clipSpaceV.z < 0.99975) ;
		if (isInside == true) {
			frustumCulled = false;
			break;
		}
	}

	if(frustumCulled == false){
		const uint UNIQUE_IDX = atomicAdd(commands[con_n].instanceCount, 1);
	    currValidInstanceProps[UNIQUE_IDX+commands[con_n].baseInstance] = rawInstanceProps[idx] ;
		validRotCol0[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol0[idx] ;
		validRotCol1[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol1[idx] ;
		validRotCol2[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol2[idx] ;
		validRotCol3[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol3[idx] ;
	}
}
