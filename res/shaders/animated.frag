#version 430

// OUTPUT
// ------

out vec4 FragColor;

// STRUCTS
// -------

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

struct Material {
    vec3 color;
    sampler2D diffuse;
    sampler2D roughness;
    bool opacity;
};

// CONSTANTS
// ---------

#define NR_POINT_LIGHTS 32
#define NR_DIRECTIONAL_LIGHTS 32
#define NR_SPOT_LIGHTS 32

// SHADER PASSED VALUES
// --------------------

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

// EXTERNALLY SET VERIABLES
// ------------------------

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess = 32.0f;
uniform vec3 viewPos;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 objectColor = vec3(1, 1, 1);
uniform samplerCube skybox;
uniform bool isReflective = false;

uniform Material objectMaterial;

// FORWARD DECLARATIONS
// --------------------

vec3 ApplyMaterial(Material material, vec3 fragPos);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// MAIN
// ----

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = {0.0f, 0.0f, 0.0f};

    // phase 1: directional lights
    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++){
        if(directionalLights[i].isActive) {
            result += CalculateDirectionalLight(directionalLights[i], norm, viewDir);
        }
    }

    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointLights[i].isActive){
            result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    // phase 3: spot lights
    for(int i = 0; i < NR_SPOT_LIGHTS; i++){
        if(spotLights[i].isActive){
            result += CalculateSpotLight(spotLights[i], norm, FragPos, viewDir);
        }
    }
    result = result * objectColor;
    FragColor = vec4(result, 1.0f);


    //Apply reflection
    //TODO: fix setting bool
    //    if(isReflective) {
    if(false) {
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, norm);
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
    //    else {
    //        result = ApplyMaterial(objectMaterial, FragPos);
    //    }
}

// LIGHT FUNCTIONS
// ---------------

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

// MATERIAL FUNCTIONS
// ------------------

vec3 ApplyMaterial(Material material, vec3 fragPos)
{
    return vec3(1);
}