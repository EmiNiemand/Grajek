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
    float shininess;
    float reflection;
    float refraction;
};

// CONSTANTS
// ---------

#define NR_POINT_LIGHTS 32
#define NR_DIRECTIONAL_LIGHTS 32
#define NR_SPOT_LIGHTS 32

#define NR_DIFFUSE 1
#define NR_SPECULAR 1
#define NR_NORMAL 1
#define NR_HEIGHT 1

// SHADER PASSED VALUES
// --------------------

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec2 vsUV;

// UNIFORMS
// --------

layout(binding = 0) uniform samplerCube skybox;
layout(binding = 1) uniform sampler2D texture_diffuse[NR_DIFFUSE];
layout(binding = 2) uniform sampler2D texture_specular[NR_SPECULAR];
// TODO: Apply normal and height maps
layout(binding = 3) uniform sampler2D texture_normal[NR_NORMAL];
layout(binding = 4) uniform sampler2D texture_hieght[NR_HEIGHT];

// EXTERNALLY SET VARIABLES
// ------------------------

uniform vec3 viewPos;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform Material material = Material(vec3(1, 1, 1), 32.0f, 0.0f, 0.0f);

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
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);

    // phase 1: directional lights
    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++){
        if(directionalLights[i].isActive) {
            result += CalculateDirectionalLight(directionalLights[i], N, V);
        }
    }

    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointLights[i].isActive){
            result += CalculatePointLight(pointLights[i], N, FragPos, V);
        }
    }

    // phase 3: spot lights
    for(int i = 0; i < NR_SPOT_LIGHTS; i++){
        if(spotLights[i].isActive){
            result += CalculateSpotLight(spotLights[i], N, FragPos, V);
        }
    }
    result = result * material.color;

    //Apply reflection
    if(material.reflection > 0.001) {
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, N);
        result = mix(result, texture(skybox, R).rgb, material.reflection);
    }
    if(material.refraction > 0.001) {
        // How much light bends
        float ratio = 1.00 / 1.52;
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = refract(I, normalize(Normal), ratio);
        result = mix(result, texture(skybox, R).rgb, material.refraction);
    }

    //cel shading
    float intensity = max(dot(-normalize(directionalLights[0].direction), N), 0.0);

    if (intensity > 0.9) {
        intensity = 0.8;
    }
    else if (intensity > 0.7) {
        intensity = 0.6;
    }
    else if (intensity > 0.5) {
        intensity = 0.4;
    }
    else if (intensity > 0.3) {
        intensity = 0.25;
    }
    else if (intensity > 0.01) {
        intensity = 0.1;
    }
    else {
        intensity = 0.0;
    }


    vec3 celColor = vec3(intensity, intensity, intensity);
    vec3 color = vec3(0.8, 0.6, 0.5);
    result = result + result * celColor;

    //     rim light
    vec3 rimLight = result * pow(max(0, (1 - dot(normalize(viewPos), N))), 4);

    result = result + rimLight;

    FragColor = vec4(result, 1.0f);
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
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    // TODO: will not work for multiple textures
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse[0], TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse[0], TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular[0], TexCoords)) * light.color;
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
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    // TODO: will not work for multiple textures
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse[0], TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse[0], TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular[0], TexCoords)) * light.color;
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
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    // TODO: will not work for multiple textures
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse[0], TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse[0], TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(texture_specular[0], TexCoords)) * light.color;
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