#version 430
out vec4 FragColor;

struct DirectionalLight {
    bool isActive;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

struct PointLight {
    bool isActive;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

struct SpotLight {
    bool isActive;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

#define NR_POINT_LIGHTS 32
#define NR_DIRECTIONAL_LIGHTS 32
#define NR_SPOT_LIGHTS 32

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess = 32.0f;
uniform vec3 viewPos;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 objectColor = vec3(1, 1, 1);

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = {0.0f, 0.0f, 0.0f};

    // phase 1: directional lighting
    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++){
        if(directionalLights[i].isActive == true) {
            result += CalculateDirectionalLight(directionalLights[i], norm, viewDir);
        }
    }

    // phase 2: point light
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointLights[i].isActive == true){
            result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    // phase 3: spot lights
    for(int i = 0; i < NR_SPOT_LIGHTS; i++){
        if(spotLights[i].isActive == true){
            result += CalculateSpotLight(spotLights[i], norm, FragPos, viewDir);
        }
    }
    result = result * objectColor;
    FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords)) * light.color;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords)) * light.color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords)) * light.color;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}