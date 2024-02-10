#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 lightColor;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct PointLight{
    vec3 position;
    vec3 lightColor;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;

uniform vec3 uViewPos; 
uniform vec3 uLightColor;

uniform sampler2D uDiffMap1;
uniform sampler2D uSpecMap1;

uniform DirLight dirLight;
uniform SpotLight spotLight[4];
uniform PointLight pointLights[14];

uniform Material material;
uniform int isMaterial;

uniform bool nightVision;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    

    vec3 norm = normalize(chNormal);
    vec3 viewDir = normalize(uViewPos - chFragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < 3; i++)
        result += CalcSpotLight(spotLight[i], norm, chFragPos, viewDir);
    for(int i = 0; i < 14; i++)
        result += CalcPointLight(pointLights[i], norm, chFragPos, viewDir);

    if(nightVision){
        result += CalcSpotLight(spotLight[3], norm, chFragPos, viewDir);
        vec3 tintColor = vec3(0.0,1.0,0.0);
        result *= tintColor;        
    }
    FragColor = vec4(result, 1.0);
    
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    if(isMaterial == 1){
        ambient = light.ambient * material.ambient * light.lightColor;
        diffuse = light.diffuse * diff * material.diffuse * light.lightColor;
        specular = light.specular * spec * material.specular * light.lightColor;
    }else{
        ambient = light.ambient * vec3(texture(uDiffMap1, chUV)) *light.lightColor;
        diffuse = light.diffuse * diff * vec3(texture(uDiffMap1, chUV)) * light.lightColor;
        specular = light.specular * spec * vec3(texture(uSpecMap1, chUV)) * light.lightColor;
    }
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    if(isMaterial == 1){
        ambient = light.ambient * material.ambient;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }else{
        ambient = light.ambient * vec3(texture(uDiffMap1, chUV));
        diffuse = light.diffuse * diff * vec3(texture(uDiffMap1, chUV));
        specular = light.specular * spec * vec3(texture(uSpecMap1, chUV));
    }

    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    if(isMaterial == 1){
        ambient = light.ambient * material.ambient * light.lightColor;
        diffuse = light.diffuse * diff * material.diffuse * light.lightColor;
        specular = light.specular * spec * material.specular * light.lightColor;
    }else{
        ambient = light.ambient * vec3(texture(uDiffMap1, chUV)) *light.lightColor;
        diffuse = light.diffuse * diff * vec3(texture(uDiffMap1, chUV)) * light.lightColor;
        specular = light.specular * spec * vec3(texture(uSpecMap1, chUV)) * light.lightColor;
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
