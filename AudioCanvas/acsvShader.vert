#version 450 core

const vec2 positions[6] = vec2[](
    vec2(1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0)
);

void main(void) {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}