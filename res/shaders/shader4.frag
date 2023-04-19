#version 430

// OUTPUT
// ------

out vec4 FragColor;

// STRUCTS
// -------

struct Material {
    vec3 color;
    float shininess;
    float reflection;
    float refraction;
};

// SHADER PASSED VALUES
// --------------------

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

// UNIFORMS
// --------
uniform samplerCube skybox;

// EXTERNALLY SET VARIABLES
// ------------------------
uniform vec3 viewPos;

uniform Material material = Material(vec3(1, 1, 1), 32.0f, 0.0f, 0.0f);

// GLOBAL VARIABLE
float shadow;

// MAIN
// ----

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f);

    if(material.reflection > 0.001) {
        vec3 I = -V;
        vec3 R = reflect(I, N);
        result = mix(result, texture(skybox, R).rgb, material.reflection);
    }
    if(material.refraction > 0.001) {
        // How much light bends
        float ratio = 1.00 / 1.52;
        vec3 I = -V;
        vec3 R = refract(I, N, ratio);
        result = mix(result, texture(skybox, R).rgb, material.refraction);
    }
    FragColor = vec4(result, 1.0f);
}
