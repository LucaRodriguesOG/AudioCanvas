#version 450 core

out vec4 outColor;

uniform float deltaTime;
uniform float programTime;
uniform ivec2 canvasResolution;

uniform sampler1D channel01;

float GrabSample(float freq) {
	return texture(channel01, freq).r;
}

vec3 hsb2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 palette(float t) {
	float brightness = 0.5;
	vec3 a = (vec3(0.962, 0.922, 0.977) * 0.5);
	vec3 b = (vec3(0.484, 0.670, 0.223) * 0.5);
	vec3 c = (vec3(1.295, 1.421, 0.083) * 0.5);
	vec3 d = (vec3(4.610, 5.642, 1.366) * 0.5);

	return a + b*cos(6.28318*(c*t+d));
}
		
void main(void) {
	vec2 uv = (gl_FragCoord.xy * 2.0 - canvasResolution.xy) / canvasResolution.y;

	vec2 uv0 = uv;

	uv *= 2. + clamp(GrabSample(0.01) * 0.5, 0., 1.);
	uv = fract(uv);
	uv -= 0.5;

	float d = length(uv);

	vec3 col = palette(length(uv0) + sin(programTime*0.1));

	d = sin(d * 8. + programTime)/8.;
	d = abs(d);

	d = 0.04 / d;

	col *= pow(d, GrabSample(0.14) * 0.4);

	outColor = vec4(1-col, 1.0);
}