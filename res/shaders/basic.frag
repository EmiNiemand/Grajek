#version 430

// OUTPUT
// ------

layout (location = 0) out vec4 screenTexture;
layout (location = 1) out vec4 texturePosition;
layout (location = 2) out vec4 textureNormal;
layout (location = 3) out vec4 textureColor;

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
in vec4 FragPosLightSpace;

// SAMPLERS
// --------
uniform sampler2D shadowMap;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse[NR_DIFFUSE];
uniform sampler2D texture_specular[NR_SPECULAR];
// TODO: Apply normal and height maps
uniform sampler2D texture_normal[NR_NORMAL];
uniform sampler2D texture_height[NR_HEIGHT];

// EXTERNALLY SET VARIABLES
// ------------------------
uniform vec3 viewPos;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform Material material = Material(vec3(1, 1, 1), 32.0f, 0.0f, 0.0f);
uniform float gamma;

// FORWARD DECLARATIONS
// --------------------

vec3 ApplyMaterial(Material material, vec3 fragPos);
vec3[3] CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3[3] CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3[3] CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

// GLOBAL VARIABLE
float shadow;

// MAIN
// ----

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    texturePosition = vec4(FragPos, 1.0f);
    textureNormal = vec4(N, 1.0f);

    vec3 result = vec3(0.0f);
    vec3 shadowResult = vec3(0.0f);

    vec3[3] lightSettings;

    shadow = ShadowCalculation(FragPosLightSpace);

    // phase 1: directional lights
    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++){
        if(directionalLights[i].isActive) {
            lightSettings = CalculateDirectionalLight(directionalLights[i], N, V);
            result += lightSettings[0] + lightSettings[1] + lightSettings[2];
            shadowResult += lightSettings[0] + (1 - shadow) * (lightSettings[1] + lightSettings[2]);
        }
    }

    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointLights[i].isActive){
            lightSettings = CalculatePointLight(pointLights[i], N, FragPos, V);
            result += lightSettings[0] + lightSettings[1] + lightSettings[2];
            shadowResult += lightSettings[0] + (1 - shadow) * (lightSettings[1] + lightSettings[2]);
        }
    }

    // phase 3: spot lights
    for(int i = 0; i < NR_SPOT_LIGHTS; i++){
        if(spotLights[i].isActive){
            lightSettings = CalculateSpotLight(spotLights[i], N, FragPos, V);
            result += lightSettings[0] + lightSettings[1] + lightSettings[2];
            shadowResult += lightSettings[0] + (1 - shadow) * (lightSettings[1] + lightSettings[2]);
        }
    }
    result = result * material.color;
    shadowResult = shadowResult * material.color;

    //cel shading
    float intensity = max(dot(-normalize(directionalLights[0].direction), N), 0.0);

    if (intensity > 0.9) {
        intensity = 0.8;
    }
    else if (intensity > 0.7) {
        intensity = 0.65;
    }
    else if (intensity > 0.5) {
        intensity = 0.50;
    }
    else if (intensity > 0.3) {
        intensity = 0.35;
    }
    else if (intensity > 0.01) {
        intensity = 0.20;
    }
    else {
        intensity = 0.0;
    }


    vec3 celColor = vec3(intensity, intensity, intensity);

    result = result + result * celColor;
    shadowResult = shadowResult + shadowResult * celColor;

    // Rim light
    float rimLight = pow(max(0, (1 - dot(normalize(-directionalLights[0].direction), N))), 1.5);

    result = result + result * rimLight;
    shadowResult = shadowResult + shadowResult * rimLight;

    if(material.reflection > 0.001) {
        vec3 I = -V;
        vec3 R = reflect(I, N);
        result = mix(result, texture(skybox, R).rgb, material.reflection);
        shadowResult = mix(shadowResult, texture(skybox, R).rgb, material.reflection);
    }
    if(material.refraction > 0.001) {
        // How much light bends
        float ratio = 1.00 / 1.52;
        vec3 I = -V;
        vec3 R = refract(I, N, ratio);
        result = mix(result, texture(skybox, R).rgb, material.refraction);
        shadowResult = mix(shadowResult, texture(skybox, R).rgb, material.refraction);
    }

    textureColor = vec4(result, 1.0f);
    screenTexture = vec4(shadowResult, 1.0f);
}

// LIGHT FUNCTIONS
// ---------------

vec3[3] CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
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
    vec3[3] lightSettings = {ambient, diffuse, specular};
    return lightSettings;
}

// calculates the color when using a point light.
vec3[3] CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    vec3[3] lightSettings = {ambient, diffuse, specular};
    return lightSettings;
}

// calculates the color when using a spot light.
vec3[3] CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    vec3[3] lightSettings = {ambient, diffuse, specular};
    return lightSettings;
}

// MATERIAL FUNCTIONS
// ------------------

vec3 ApplyMaterial(Material material, vec3 fragPos)
{
    return vec3(1);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
//    vec3 normal = normalize(Normal);
//    vec3 lightDir = -directionalLights[0].direction;
//    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // SKIP CALCULATING BIAS BECAUSE BIAS = 0 WORKS BEST FOR OUR SCENE

    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            // SKIP BIAS
//            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
            shadow += currentDepth > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}