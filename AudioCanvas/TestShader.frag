#version 460 core

in vec3 fragColor;

out vec4 outColor;

uniform float deltaTime;
uniform float programTime;

void main(void) {
    outColor = vec4(fragColor, 1.0);
}