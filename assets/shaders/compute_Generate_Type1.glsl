#version 460 core
layout(local_size_x = 32, local_size_y = 32) in;
layout(binding = 0, rgba16f) uniform image2D ImgIn;

uniform float Type;
uniform float Seed;
uniform vec2 Size;

vec2 hash2(vec2 p);
float noise(in vec2 p);
vec3 Voronoi(vec2 CurrentCoord, vec2 coord, float offset);

void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec4 sampled = imageLoad(ImgIn, pixel_coords);
	vec2 center_pixel_coords = pixel_coords - (Size / 2);
	//ivec2 texCoords = ivec2(4096 / 2 * MousePos.x, 4096 / 2 * MousePos.y);


	if (Type == 0) {
		imageStore(ImgIn, pixel_coords, vec4(0.0, 0.0, 0.0, 0.0));
	}
	if (Type == 1) {
		imageStore(ImgIn, pixel_coords, sampled + vec4(0.0, noise((pixel_coords / 100.0) + sampled.g + (Seed * 235)), 0.0, 0.0));
	}
	if (Type == 2) {
		imageStore(ImgIn, pixel_coords, sampled + vec4(noise((pixel_coords / 100.0) + (Seed * 378)) / 5, 0.0, 0.0, 1.0));
	}
	if (Type == 3) {
		if ((noise((pixel_coords / 100.0) + (Seed * 123))) > 0.0)
		{
			imageStore(ImgIn, pixel_coords, vec4(0.0, 0.0, 0.0, 0.0));
		}

	}
	
	float a = 0;
	float pixels = 16;
	vec2 fraction = Size / pixels;
	vec2 clampCoords = floor(center_pixel_coords / fraction) + 0.5;

	vec3 outV = Voronoi(vec2(center_pixel_coords) / fraction, clampCoords, fraction.x);

	a = (noise(clampCoords) + 1) / 2.0;
	a *= 1 - (distance(clampCoords, vec2(0)) / (pixels / 2));
	a = clamp(a, 0, 1);

	vec2 g = normalize(hash2(clampCoords));
	imageStore(ImgIn, pixel_coords, vec4(g.x, g.y, 0.0, 1));
	//imageStore(ImgIn, pixel_coords, vec4(outV.y / (pixels / 2), outV.z / (pixels / 2), 1 - (distance(clampCoords, vec2(0)) / (pixels / 2)), 1));
}

vec3 Voronoi(vec2 CurrentCoord, vec2 coord, float offset) {
	vec2 points[9];

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			int index = (x + 1) + ((y + 1) * 3);
			points[index] = vec2(x, y) * offset;
		}
	}

	vec2 closest;
	float dist = 999999;

	for (int i = 0; i < 9; i++) {
		vec2 point = (coord + points[i]) + (normalize(hash2(coord + points[i])) * 4000);
		float currentDist = distance(CurrentCoord, point);
		if (currentDist < dist) {
			dist = currentDist;
			closest = coord + points[i];
		}
	}

	
	return vec3(dist, closest);
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