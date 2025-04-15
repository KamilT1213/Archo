#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform float factor;
uniform float allTime;

uniform vec2 u_ScreenSize;

float pi = 3.1415;

vec2 hash2(vec2 p);
float noise(in vec2 p);

void old();
mat2 rotate2d(float _angle);

void main()
{
	old();
	//float ratio = u_ScreenSize.x / u_ScreenSize.y;

	//float Modulation = 10;
	//float adjustedTime = mod(allTime, Modulation);
	//float adjustedPi = (adjustedTime * pi) / (Modulation / 2);


	//vec2 centerCoords = texCoords - 0.5f;
	//centerCoords.x *= ratio;

	////vec2 point = vec2(0, 1);//)vec2(sin(adjustedPi), cos(adjustedPi));
	////point *= 0.6;
	////point = point * rotate2d(adjustedPi * 100 * distance(vec2(0),centerCoords));

	//centerCoords = centerCoords * rotate2d(adjustedPi);
	//vec2 centerCoords2 = centerCoords * rotate2d((adjustedPi * 5) - ( distance(vec2(0), centerCoords) * 5));


	//float f = noise((centerCoords * u_ScreenSize * 0.25) + 0.5) - abs(noise((centerCoords * u_ScreenSize * 0.01) + 0.5));
	//float f2 = noise((centerCoords2 * u_ScreenSize * 0.25) + 0.5) - abs(noise((centerCoords2 * u_ScreenSize * 0.01) + 0.5));
	//if (distance(vec2(0), centerCoords) < 0.5) f = f2;
	////if (distance(vec2(0), centerCoords) < 0.4) f -= distance(vec2(0), centerCoords) *  (1 / 0.4);
	////if (distance(centerCoords, point) < 0.05) {
	////	f = 1;
	////}

	//colour = vec4(f, f, f, 1.0);
	//old();



}

void old() {
	vec2 texelSize = 1.0 / u_ScreenSize;
	float a = min(u_ScreenSize.x, u_ScreenSize.y);
	float b = max(u_ScreenSize.x, u_ScreenSize.y);
	float c = a / b;

	vec4 total = vec4(1.0);

	float bTotal;

	for (int i = 0; i < 5; i++) {
		bTotal += noise((texCoords + vec2(i) + vec2(sin((allTime * 3) + (texCoords.y * 5)) / 100, allTime / 10)) * (i + 1) * 42.532);
	}
	bTotal = ((bTotal / 5) + 1) / 2.0;
	bTotal = ((floor(bTotal * 20) + 0.5) / 20);

	vec2 normedTex = texCoords;
	vec2 dist = vec2(normedTex.x - 0.5f, normedTex.y - 0.5f);

	if (u_ScreenSize.x > u_ScreenSize.y) {
		dist.x *= (u_ScreenSize.x / u_ScreenSize.y);
	}
	else {
		dist.y *= (u_ScreenSize.y / u_ScreenSize.x);
	}
	float d = c;
	float s = 1;

	dist *= s;


	float distance = (max(abs(dist.x), abs(dist.y)) - (d * s));
	bTotal += clamp(((distance) * 2), -1.0, 0.3);
	total = vec4(vec3(0.3, bTotal, 0.6), 1.0);


	colour = total;
};

mat2 rotate2d(float _angle) {
	return mat2(cos(_angle), -sin(_angle),
		sin(_angle), cos(_angle));
}
vec2 hash2(vec2 p)
{
	p = vec2(dot(p, vec2(127.1, 311.7)),
		dot(p, vec2(269.5, 183.3)));

	return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise(in vec2 p)
{
	vec2 i = floor(p);
	vec2 f = fract(p);

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(dot(hash2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
		dot(hash2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
		mix(dot(hash2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
			dot(hash2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

