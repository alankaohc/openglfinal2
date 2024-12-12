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

// top 
uniform vec3 ori_top;
uniform vec3 t_top;
uniform vec3 b_top;
uniform vec3 n_top;
// btm
uniform vec3 ori_btm;
uniform vec3 t_btm;
uniform vec3 b_btm;
uniform vec3 n_btm;
// right
uniform vec3 ori_right;
uniform vec3 t_right;
uniform vec3 b_right;
uniform vec3 n_right;
// left
uniform vec3 ori_left;
uniform vec3 t_left;
uniform vec3 b_left;
uniform vec3 n_left;
// near
uniform vec3 ori_near;
uniform vec3 t_near;
uniform vec3 b_near;
uniform vec3 n_near;
// far
uniform vec3 ori_far;
uniform vec3 t_far;
uniform vec3 b_far;
uniform vec3 n_far;

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

	// top
	vec3 P_top = center.xyz - ori_top;
	vec3 P_prime_top = dot(P_top, t_top)*t_top + dot(P_top, b_top)*b_top;
	vec3 v_top = P_top - P_prime_top;
	float d_top;
	if (dot(v_top, n_top) >= 0) {
		d_top =  1 * abs(distance(P_top, P_prime_top));
	} else {
		d_top = -1 * abs(distance(P_top, P_prime_top));
	}
	
	// btm
	vec3 P_btm = center.xyz - ori_btm;
	vec3 P_prime_btm = dot(P_btm, t_btm)*t_btm + dot(P_btm, b_btm)*b_btm;
	vec3 v_btm = P_btm - P_prime_btm;
	float d_btm;
	if (dot(v_btm, n_btm) >= 0) {
		d_btm =  1 * abs(distance(P_btm, P_prime_btm));
	} else {
		d_btm = -1 * abs(distance(P_btm, P_prime_btm));
	}
	// right
	vec3 P_right = center.xyz - ori_right;
	vec3 P_prime_right = dot(P_right, t_right)*t_right + dot(P_right, b_right)*b_right;
	vec3 v_right = P_right - P_prime_right;
	float d_right;
	if (dot(v_right, n_right) >= 0) {
		d_right =  1 * abs(distance(P_right, P_prime_right));
	} else {
		d_right = -1 * abs(distance(P_right, P_prime_right));
	}
	// left
	vec3 P_left = center.xyz - ori_left;
	vec3 P_prime_left = dot(P_left, t_left)*t_left + dot(P_left, b_left)*b_left;
	vec3 v_left = P_left - P_prime_left;
	float d_left;
	if (dot(v_left, n_left) >= 0) {
		d_left =  1 * abs(distance(P_left, P_prime_left));
	} else {
		d_left = -1 * abs(distance(P_left, P_prime_left));
	}
	// near
	vec3 P_near = center.xyz - ori_near;
	vec3 P_prime_near = dot(P_near, t_near)*t_near + dot(P_near, b_near)*b_near;
	vec3 v_near = P_near - P_prime_near;
	float d_near;
	if (dot(v_near, n_near) >= 0) {
		d_near =  1 * abs(distance(P_near, P_prime_near));
	} else {
		d_near = -1 * abs(distance(P_near, P_prime_near));
	}
	// far
	vec3 P_far = center.xyz - ori_far;
	vec3 P_prime_far = dot(P_far, t_far)*t_far + dot(P_far, b_far)*b_far;
	vec3 v_far = P_far - P_prime_far;
	float d_far;
	if (dot(v_far, n_far) >= 0) {
		d_far =  1 * abs(distance(P_far, P_prime_far));
	} else {
		d_far = -1 * abs(distance(P_far, P_prime_far));
	}




	bool frustumCulled = true;
	if ( d_btm < radius && d_right < radius && d_left < radius && d_near < radius && d_far < radius) {
		frustumCulled = false;
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


	/*
	vec4 v_left = center + vec4(-radius, 0, 0, 1);
	vec4 v_right = center + vec4(radius, 0, 0, 1);
	vec4 v_top = center + vec4(0, radius, 0, 1);
	vec4 v_bottom = center + vec4(0, -radius, 0, 1);
	vec4 v_near = center + vec4(0, 0, radius, 1);
	vec4 v_far = center + vec4(0, 0, -radius, 1);
	vec4 v_0 = center + vec4(-1/2*radius,  1/sqrt(2)*radius,  1/2*radius, 1);
	vec4 v_1 = center + vec4( 1/2*radius,  1/sqrt(2)*radius,  1/2*radius, 1);
    vec4 v_2 = center + vec4(-1/2*radius,  1/sqrt(2)*radius, -1/2*radius, 1);
	vec4 v_3 = center + vec4( 1/2*radius,  1/sqrt(2)*radius, -1/2*radius, 1);
	vec4 v_4 = center + vec4(-1/2*radius, -1/sqrt(2)*radius,  1/2*radius, 1);
	vec4 v_5 = center + vec4( 1/2*radius, -1/sqrt(2)*radius,  1/2*radius, 1);
    vec4 v_6 = center + vec4(-1/2*radius, -1/sqrt(2)*radius, -1/2*radius, 1);
	vec4 v_7 = center + vec4( 1/2*radius, -1/sqrt(2)*radius, -1/2*radius, 1);


	vec4 array[14];
	array[0] = v_left;
	array[1] = v_right;
	array[2] = v_top;
	array[3] = v_bottom;
	array[4] = v_near;
	array[5] = v_far;
	array[6] = v_0;
	array[7] = v_1;
	array[8] = v_2;
	array[9] = v_3;
	array[10] = v_4;
	array[11] = v_5;
	array[12] = v_6;
	array[13] = v_7;
	for (int i=0; i<14; i++) {
		vec4 clipSpaceV = viewProjMat * vec4(array[i].xyz, 1.0);
		clipSpaceV = clipSpaceV / clipSpaceV.w;
		bool isInside = (clipSpaceV.x > -1.0) && (clipSpaceV.x < 1.0) && (clipSpaceV.y > -1.0) &&
		(clipSpaceV.y < 1.0) && (clipSpaceV.z > -1.0) && (clipSpaceV.z < 0.99975) ;
		if (isInside == true) {
			frustumCulled = false;
			break;
		}
	}
	*/