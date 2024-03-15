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

float smin(float a, float b, float k) {
	float h = max(k - abs(a - b), 0.0 ) / k;
	return min(a, b) - h*h*h*k*(1.0/6.0);
}

mat2 rot2D(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	return mat2(c, -s, s, c);
}

float map(vec3 p) {
	vec3 spherePos = vec3(sin(programTime)*3., cos(programTime)*3., 0);
	float sphere = sdSphere(p - spherePos, 0.2);

	vec3 q = p;

	q = fract(p) - .5;

	float box = sdBox(q, vec3(.1));

	float ground = p.y + 0.75;

	return smin(ground, smin(sphere, box, 2.), 1.);
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
		vec3 p = ro + rd * t;               // position along the ray origin

		float d = map(p);                   // current distance

		t += d;								// increment distance

		if (d < 0.001 || t > 100.0)			// optimization
			break;
	}

	col = vec3(t * 0.05);

	outColor = vec4(col, 1.0);
}