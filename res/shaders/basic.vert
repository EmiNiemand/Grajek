#version 430
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix = mat4(0.0f);

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform vec2 texStrech = vec2(1, 1);

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1)
        continue;
        if(boneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
    }
    if (totalPosition == vec4(0.0f)) {
        FragPos = vec3(model * vec4(aPos, 1.0));
        TexCoords = vec2(aTexCoords.x * texStrech.x, aTexCoords.y * texStrech.y);
        Normal = mat3(transpose(inverse(model))) * aNormal;
        FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    else {
        FragPos = vec3(model * totalPosition);
        TexCoords = vec2(aTexCoords.x * texStrech.x, aTexCoords.y * texStrech.y);
        Normal = mat3(transpose(inverse(model))) * aNormal;
        FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
        gl_Position =  projection * view * model * totalPosition;
    }
}