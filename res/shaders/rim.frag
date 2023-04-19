#version 430

// OUTPUT
// ------

out vec4 FragColor;

// SHADER PASSED VALUES
// --------------------

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;


// EXTERNALLY SET VARIABLES
// ------------------------
uniform vec3 viewPos;


// MAIN
// ----

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 result = vec3(1.0f);

    //     rim light
    float rimLight = pow(1 - dot(V, N), 1.5);

    result = result * rimLight;

//    result = vec3(rimLight, rimLight, rimLight);

    FragColor = vec4(result, 1.0f);
}
