#version 460 core

uniform sampler2D uTextureSampler;

out vec4 fragColor;

in vec3 color;
in vec2 uv;

void main() {
    vec3 textureColor = texture(uTextureSampler, uv).rgb;
    fragColor = vec4(color * textureColor, 1);
}