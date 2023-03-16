#version 430
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z + 0.1, 1.0);
}
