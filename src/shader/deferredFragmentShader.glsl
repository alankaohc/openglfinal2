#version 430 core

in vec2 TexCoords;
out vec4 FragColor;


layout (location=0) uniform sampler2D gSpecular;
layout (location=1) uniform sampler2D gNormal;
layout (location=2) uniform sampler2D gDiffuse;
layout (location=3) uniform sampler2D gPosition;
layout (location=4) uniform sampler2D depthMap;

//uniform sampler2D gView;
uniform int flag;
uniform mat4 lightSpaceMatrix;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 N, vec3 L)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);  
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 
    
    return shadow;
}  


void main()
{            

    
    float depthValue = texture(depthMap, TexCoords).r;
    vec3 depth = vec3(depthValue);
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
   

    // Blinn-Phong shading 
    vec3 Ia = vec3(0.2, 0.2, 0.2);
    vec3 Id = vec3(0.64, 0.64, 0.64);
    vec3 Is = vec3(0.16, 0.16, 0.16);

    vec3 N = normalize(texture(gNormal, TexCoords).rgb);
    vec3 L = normalize(vec3(0.4, 0.5, 0.8)); // light direction
    //vec3 V = normalize(texture(gView, TexCoords).rgb);
    vec3 V = vec3(10.0,10.0,10.0);
    vec3 H = normalize(L + V);
 
    float shadow = ShadowCalculation(FragPosLightSpace, N, L);


    vec3 diffuse_albedo = texture(gDiffuse, TexCoords).rgb;
    vec3 specular_albedo = texture(gSpecular, TexCoords).rgb;
    float shininess_power = texture(gSpecular, TexCoords).a;
    

    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 ambient = diffuse;
    vec3 specular = pow(max(dot(N, H), 0.0), shininess_power) * specular_albedo;

    vec3 color = Ia*ambient + (1.0 - shadow) * (Id*diffuse + Is*specular);
    FragPos = normalize(FragPos) * 0.5 + 0.5;
    N = N * 0.5 + 0.5;

    color = pow(color, vec3(0.5));
    vec3 tex = texture(gDiffuse, TexCoords).rgb;
   

    if (flag == 1) {
        FragColor = vec4(depth, 1.0);
    } else if (flag == 2) {
        FragColor = vec4(N, 1.0);
    } else if (flag == 3) {
        FragColor = vec4(specular_albedo, 1.0);    
    } else if (flag == 4) {
        FragColor = vec4(color, 1.0);
    } else if (flag == 5){
        FragColor = vec4(FragPos, 1.0);
    }
}