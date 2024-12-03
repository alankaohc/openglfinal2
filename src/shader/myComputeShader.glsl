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

	if(idx >= numMaxInstance){ return; }


	vec4 clipSpaceV = viewProjMat * vec4(rawInstanceProps[idx].position.xyz, 1.0) ;
	clipSpaceV = clipSpaceV / clipSpaceV.w;
	bool frustumCulled = (clipSpaceV.x < -1.0) || (clipSpaceV.x > 1.0) || (clipSpaceV.y < -1.0) ||
	(clipSpaceV.y > 1.0) || (clipSpaceV.z < -1.0) || (clipSpaceV.z > 1.0) ;
	
//	if(true){
//		const uint UNIQUE_IDX = atomicAdd(commands[0].instanceCount, 1);
//	    currValidInstanceProps[UNIQUE_IDX] = rawInstanceProps[idx];
//	}
	if(frustumCulled == false){
		int con_n;
		if (idx>=0 && idx < commands[1].baseInstance) {
			con_n = 0;
		}
		if (idx>=commands[1].baseInstance && idx<commands[2].baseInstance) {
			con_n = 1;
		}
		if (idx>=commands[2].baseInstance && idx<commands[3].baseInstance) {
			con_n = 2;
		}
		if (idx>=commands[3].baseInstance && idx<commands[4].baseInstance) {
			con_n = 3;
		}
		if (idx>=commands[4].baseInstance) {
			con_n = 4;
		}
		const uint UNIQUE_IDX = atomicAdd(commands[con_n].instanceCount, 1);
	    currValidInstanceProps[UNIQUE_IDX+commands[con_n].baseInstance] = rawInstanceProps[idx] ;
		validRotCol0[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol0[idx] ;
		validRotCol1[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol1[idx] ;
		validRotCol2[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol2[idx] ;
		validRotCol3[UNIQUE_IDX+commands[con_n].baseInstance] = rawRotCol3[idx] ;
	}
}
