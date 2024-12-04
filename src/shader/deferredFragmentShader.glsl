#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

layout (location=0) uniform sampler2D gSpecular;
layout (location=1) uniform sampler2D gNormal;
layout (location=2) uniform sampler2D gDiffuse;
layout (location=3) uniform sampler2D gPosition;

//uniform sampler2D gView;

uniform int flag;





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
    //vec3 V = normalize(texture(gView, TexCoords).rgb);
    vec3 V = vec3(10.0,10.0,10.0);
    vec3 H = normalize(L + V);
 
    vec3 diffuse_albedo = texture(gDiffuse, TexCoords).rgb;
    vec3 specular_albedo = texture(gSpecular, TexCoords).rgb;
    float shininess_power = texture(gSpecular, TexCoords).a;
    

    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 ambient = diffuse;
    vec3 specular = pow(max(dot(N, H), 0.0), shininess_power) * specular_albedo;

    vec3 color = Ia*ambient + Id*diffuse + Is*specular;
    FragPos = normalize(FragPos) * 0.5 + 0.5;
    N = N * 0.5 + 0.5;

    color = pow(color, vec3(0.5));
    vec3 tex = texture(gDiffuse, TexCoords).rgb;



    if (flag == 1) {
        FragColor = vec4(tex, 1.0);
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