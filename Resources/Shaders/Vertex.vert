#version 460 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(inPosition.xyz, 1.0f);
    uv = inUV;
    color = inColor;
}