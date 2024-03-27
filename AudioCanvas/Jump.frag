#version 450 core

in vec3 fragColor;

out vec4 outColor;

uniform float deltaTime;
uniform float programTime;
uniform ivec2 canvasResolution;

uniform sampler1D channel01;

// helpers
vec3 GetSample(float p) {
    return texture(channel01, p).rgb;
}

float sun(vec2 uv, float battery)
{
 	float val = smoothstep(0.3, 0.29, length(uv));
    return clamp(val, 0.0, 1.0);
}

float smin(float a, float b, float k) {
    float h = max(k - abs(a-b), 0.0) / k;
    return min(a, b) - h*h*h*k*(1.0/6.0);
}

mat2 rot2D(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}


// shapes
float sdPlane(vec3 p, vec3 n, float h) {
    return dot(p, n) + h;
}

float sdSphere(vec3 p, float s) {
    return length(p) - s;
}

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdCylinder(vec3 p, float ra, float rb, float h) {
    vec2 d = vec2(length(p.xz) - (2.0 * ra) + rb, abs(p.y) - h);
    return  min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - rb;
}

float map(vec3 p) {
    // ground
    float ground = p.y + 0.75;

    //  vec3(0, 1/sqrt(2), 1/sqrt(2))
    vec3 backPosition = vec3(0, 0, 0);
    float back = sdPlane(p, normalize(vec3(0, 50, -150)), 40);
   
    // sphere
    float s = GetSample(gl_FragCoord.x / canvasResolution.x * .5).x * 20;
    vec3 spherePos = vec3(0, (sin(programTime) / 2) + 5, s + 20);
    float sphere = sdSphere(p - spherePos, 10);

    // box
    float s2 = GetSample(.1).x;
    vec3 q = p + vec3(0, 1. - s2, 0);
    q.xz = fract(p + vec3(sin(programTime * .5), 0., programTime)).xz - .5;
    q.xy *= rot2D(sin(programTime * .5));
    //q.yz *= rot2D(programTime);


    float box = sdBox(q, vec3(.1, .2, .5));


    return min(back, smin(ground, min(sphere, box), .5));
}

void main(void) {
    vec2 uv = (gl_FragCoord.xy * 2. - canvasResolution.xy) / canvasResolution.y;

    // init
    vec3 ro = vec3(0, 0, -3);               // ray origin
    vec3 rd = normalize(vec3(uv, 1));       // ray direction
    vec3 col = vec3(.1);                   // final pixel color

    float t = 0.0;                          // total distance travelled

    // Raymarching
    for (int i = 0; i < 80; i++) {
        vec3 p = ro + rd * t;                   // position along the ray origin

        float d = map(p);                       // current distance

        t += d;

        if (d < 0.001 || t > 100.0) 
            break;
    }
    
    // color
    vec2 q = gl_FragCoord.xy / canvasResolution.xy;
    if (t >= 40) {
        col = mix(vec3(.9, .3, .0), vec3(0, 0, .2), t * .01);
    }
    else if (t > 37 && gl_FragCoord.y / canvasResolution.y > 2) {
        col = vec3(1,1,1);
    }
    else if (t > 13 && (gl_FragCoord.y / canvasResolution.y) < 2) {
        col = mix(vec3(.9, .7, .0), vec3(.8, .5, .0), t * .15);
    }
    else if (t > 0){
        col = t * mix(vec3(.04, .02, .0), vec3(.0, .0, .08), 1 - (t * .08)); // sqrt(t) * .01
    }

    outColor = vec4(col, 1.0);
}