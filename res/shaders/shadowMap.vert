#version 430
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

const int MAX_BONES = 40;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform bool isAnimated = false;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    if (isAnimated == true) {
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

        gl_Position = lightSpaceMatrix * model * totalPosition;
    }
    else {
        gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0f);
    }
}
