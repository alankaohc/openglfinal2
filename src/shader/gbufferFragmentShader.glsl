#version 450 core

in vec3 f_worldVertex ;
in vec3 f_worldNormal ;
in vec3 f_uv ;
in vec3 f_view;

uniform sampler2DArray albedoTextureArray ;

//layout (location = 0) out vec3 gPosition;
layout (location = 0) out vec4 gSpecular;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec3 gPosition;

//layout (location = 4) out vec3 gView;



void main()
{
	vec4 texel = texture(albedoTextureArray, f_uv) ;
	if(texel.a < 0.5){
		discard ;
	}
	// store the fragment position vector in the first gbuffer texture
    gPosition = f_worldVertex;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(f_worldNormal);
    // and the diffuse per-fragment color
    gDiffuse = texel;
    // store specular + shininess
    gSpecular = vec4(0.0, 0.0, 0.0, 1.0);
    // view
    //gView = f_view;
}



