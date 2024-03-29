#version 450 core

in vec3 fragColor;

out vec4 outColor;

uniform float deltaTime;
uniform float programTime;
uniform ivec2 canvasResolution;

uniform sampler1D channel01;

// Code referenced from https://thebookofshaders.com/06/
// Modified main function to translate side-to-side.

vec3 rgb2hsb( in vec3 c ){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main(void) {
    vec3 color = vec3(1.0, 0.0, 1.0);
    vec2 st = vec2(gl_FragCoord.x + (programTime*200.0), gl_FragCoord.y) / canvasResolution;

   

    if (texture(channel01, gl_FragCoord.x / canvasResolution.x * 2.0).r * 0.5 > abs(gl_FragCoord.y / canvasResolution.y - 0.5)) {
        color = vec3(0.34) + vec3(gl_FragCoord.y / canvasResolution.y, 0., gl_FragCoord.x / canvasResolution.x);
    } else {
        color = vec3(0.1);
    }


    outColor = vec4(color, 1.0);
}