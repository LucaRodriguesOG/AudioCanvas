#version 450 core

in vec3 fragColor;

out vec4 outColor;

uniform float deltaTime;
uniform float programTime;
uniform ivec2 canvasResolution;

uniform sampler1D channel01;


void main(void) {
    vec3 color = vec3(1.0, 0.0, 1.0);

    if (texture(channel01, gl_FragCoord.x / canvasResolution.x).r * 10.0 > gl_FragCoord.y / canvasResolution.y) {
        color = vec3(1.0);
    } else {
        color = vec3(0.0);
    }


    outColor = vec4(color, 1.0);
}