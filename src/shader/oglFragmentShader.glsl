#version 430 core

in vec3 f_viewVertex ;
in vec3 f_uv ;

//layout (location = 0) out vec4 fragColor ;
layout(location = 2) uniform int pixelProcessId;
layout(location = 4) uniform sampler2D albedoTexture ;


layout (location = 0) out vec4 gSpecular;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec3 gPosition;

in vec3 f_worldVertex ;
in vec3 f_worldNormal ;


vec4 withFog(vec4 color){
	const vec4 FOG_COLOR = vec4(0.0, 0.0, 0.0, 1) ;
	const float MAX_DIST = 400.0 ;
	const float MIN_DIST = 350.0 ;
	
	float dis = length(f_viewVertex) ;
	float fogFactor = (MAX_DIST - dis) / (MAX_DIST - MIN_DIST) ;
	fogFactor = clamp(fogFactor, 0.0, 1.0) ;
	fogFactor = fogFactor * fogFactor ;
	
	vec4 colorWithFog = mix(FOG_COLOR, color, fogFactor) ;
	return colorWithFog ;
}


void terrainPass(){
	vec4 texel = texture(albedoTexture, f_uv.xy) ;
	//fragColor = withFog(texel); 
	//fragColor.a = 1.0;	
	gDiffuse = withFog(texel);
	gDiffuse.a = 1.0;
	gPosition = f_worldVertex;
	gNormal = f_worldNormal;
	gSpecular = vec4(0.0, 0.0, 0.0, 1.0);
}

void pureColor(){
	//fragColor = withFog(vec4(1.0, 0.0, 0.0, 1.0)) ;
	gDiffuse = withFog(vec4(1.0, 0.0, 0.0, 1.0)) ;
	gPosition = f_worldVertex;
	gNormal = f_worldNormal;
	gSpecular = vec4(0.0, 0.0, 0.0, 1.0);
}

void main(){	
	if(pixelProcessId == 5){
		pureColor() ;
	}
	else if(pixelProcessId == 7){
		terrainPass() ;
	}
	else{
		pureColor() ;
	}
}