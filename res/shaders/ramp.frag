#version 430

// OUTPUT
// ------

out vec4 FragColor;

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

// UNIFORMS
// --------
uniform sampler2D rampMap;
uniform sampler2D shadowMap;
uniform sampler2D texture_diffuse[NR_DIFFUSE];
uniform sampler2D texture_specular[NR_SPECULAR];
// TODO: Apply normal and height maps
uniform sampler2D texture_normal[NR_NORMAL];
uniform sampler2D texture_height[NR_HEIGHT];

// EXTERNALLY SET VARIABLES
// ------------------------
uniform vec3 viewPos;


// MAIN
// ----

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // Map value from [-1, 1] to [0, 1]
    float rampCoord = dot(V, N) * 0.5 + 0.5;

    vec4 ramp = texture2D(rampMap, vec2(rampCoord, 0.5f));

    FragColor =  ramp;
}