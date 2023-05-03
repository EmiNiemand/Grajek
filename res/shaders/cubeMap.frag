#version 430

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float gamma;

void main()
{
    FragColor = texture(skybox, TexCoords);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}
