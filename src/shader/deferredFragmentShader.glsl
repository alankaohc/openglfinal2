#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

layout (location=0) uniform sampler2D gSpecular;
layout (location=1) uniform sampler2D gNormal;
layout (location=2) uniform sampler2D gDiffuse;
layout (location=3) uniform sampler2D gPosition;
uniform int flag;

// CSM

uniform sampler2DArray shadowMap;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform float farPlane;

uniform mat4 view;

layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1


uniform vec4 corners[3];

/*
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
*/
int index = -1;
// vertex shader ¥u¶Ç world space coord¡A
float ShadowCalculation(vec3 fragPosWorldSpace, mat4 view, vec3 N)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            index = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
        index = 2;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = N;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    const float biasModifier = 0.5f;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return shadow;
}



void main()
{            
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    // Blinn-Phong shading 
    vec3 Ia = vec3(0.2, 0.2, 0.2);
    vec3 Id = vec3(0.64, 0.64, 0.64);
    vec3 Is = vec3(0.16, 0.16, 0.16);
    vec3 N = normalize(texture(gNormal, TexCoords).rgb);
    vec3 L = normalize(vec3(0.4, 0.5, 0.8)); // light direction
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);
 
    vec3 diffuse_albedo = texture(gDiffuse, TexCoords).rgb;
    vec3 specular_albedo = texture(gSpecular, TexCoords).rgb;
    float shininess_power = texture(gSpecular, TexCoords).a;
    
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 ambient = diffuse;
    vec3 specular = pow(max(dot(N, H), 0.0), shininess_power) * specular_albedo;

    // calculate shadow
    float shadow = ShadowCalculation(FragPos, view, N);   
    
    vec3 lighting = Ia*ambient + (1.0 - shadow*2.0) * (Id*diffuse + Is*specular);
    


    // cascade visualization
    
    
    mat4 M0 = lightSpaceMatrices[0];
    mat4 M1 = lightSpaceMatrices[1];
    mat4 M2 = lightSpaceMatrices[2];
    vec4 p0 = M0 * vec4(FragPos, 1.0);
    vec4 p1 = M1 * vec4(FragPos, 1.0);
    vec4 p2 = M2 * vec4(FragPos, 1.0);
    p0 = p0 / p0.w;
    p1 = p1 / p1.w;
    p2 = p2 / p2.w;

    bool isInside0 = (p0.x > -1.0) && (p0.x < 1.0) && 
                     (p0.y > -1.0) && (p0.y < 1.0) && 
                     (p0.z > -1.0) && (p0.z < 1.0);
    bool isInside1 = (p1.x > -1.0) && (p1.x < 1.0) && 
                     (p1.y > -1.0) && (p1.y < 1.0) && 
                     (p1.z > -1.0) && (p1.z < 1.0);
    bool isInside2 = (p2.x > -1.0) && (p2.x < 1.0) && 
                     (p2.y > -1.0) && (p2.y < 1.0) && 
                     (p2.z > -1.0) && (p2.z < 1.0);
    
  
    if (isInside0) {
        lighting = mix(lighting, vec3(1.0, 0.0, 0.0), 0.2); 
    } else if (isInside1) {
        lighting = mix(lighting, vec3(0.0, 1.0, 0.0), 0.2); 
    } else if (isInside2) {
        lighting = mix(lighting, vec3(0.0, 0.0, 1.0), 0.2);
    }
    
/* 
    if (index == 0) {
        lighting = mix(lighting, vec3(1.0, 0.0, 0.0), 0.2); 
    } else if (index == 1) {
        lighting = mix(lighting, vec3(0.0, 1.0, 0.0), 0.2);
    } else if (index == 2) {
        lighting = mix(lighting, vec3(0.0, 0.0, 1.0), 0.2);
    }
    */
    // final
    vec3 color = pow(lighting, vec3(0.3));
    FragPos = normalize(FragPos) * 0.5 + 0.5;
    N = N * 0.5 + 0.5;
    vec3 tex = texture(gDiffuse, TexCoords).rgb;

    if (flag == 1) {
        FragColor = vec4(color, 1.0);
    } else if (flag == 2) {
        FragColor = vec4(N, 1.0);
    } else if (flag == 3) {
        FragColor = vec4(specular_albedo, 1.0);    
    } else if (flag == 4) {
        FragColor = vec4(tex, 1.0);
    } else if (flag == 5){
        FragColor = vec4(FragPos, 1.0);
    }
}