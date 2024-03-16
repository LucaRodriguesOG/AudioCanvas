#version 450 core

in vec3 fragColor;

out vec4 outColor;

uniform float deltaTime;
uniform float programTime;
uniform ivec2 canvasResolution;

uniform sampler1D channel01;

float sdSphere(vec3 p, float s) {
    return length(p) - s;
}

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float map(vec3 p) {
    // vec3 spherePos = vec3(sin(programTime)*3.0, cos(programTime) * 1.5, 0);
    float s = texture(channel01, gl_FragCoord.x / canvasResolution.x * .5).r * 25;
    vec3 spherePos = vec3(0, 0, s);
    float sphere = sdSphere(p - spherePos, 1.0);

    // float box = sdBox(p, vec3(0.75));

    return sphere;
}

void main(void) {
    vec2 uv = (gl_FragCoord.xy * 2. - canvasResolution.xy) / canvasResolution.y;

    // init
    vec3 ro = vec3(0, 0, -3);               // ray origin
    vec3 rd = normalize(vec3(uv, 1));       // ray direction
    vec3 col = vec3(0.1);                   // final pixel color

    float t = 0.0;                          // total distance travelled

    // Raymarching
    for (int i = 0; i < 80; i++) {
        vec3 p = ro + rd * t;                   // position along the ray origin

        float d = map(p);                       // current distance

        t += d;

        if (d < 0.001 || t > 100.0) 
            break;
    }

    col = vec3(t * 0.2);

    outColor = vec4(col, 1.0);
}