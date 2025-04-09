#version 460 core
layout(local_size_x = 32, local_size_y = 32) in;
layout(binding = 0, rgba16f) uniform image2D GroundImg;
layout(binding = 1, rgba16f) uniform image2D GroundImgHold;

uniform float Reset;
uniform float ResetWave;
uniform vec2 MousePos;
uniform float action;
uniform float subBy;
uniform float digging;

vec2 hash2(vec2 p);
float noise(in vec2 p);

void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec4 sampled = imageLoad(GroundImg, pixel_coords);
	ivec2 texCoords = ivec2(4096 / 2 * MousePos.x, 4096 / 2 * MousePos.y);
	vec4 middleSampled = imageLoad(GroundImg, texCoords);

	if (Reset > 0) {
		float noiseLevel = noise(vec2(pixel_coords.x, pixel_coords.y) / 250);
		noiseLevel /= 1;
		noiseLevel += 1;
		noiseLevel /= 10;
		sampled.x = clamp(sampled.x, 0, ResetWave - (noiseLevel * (1 - ResetWave)));
		sampled.z = 1;
		sampled.a = 1;
		if (ResetWave <= 0) {
			sampled.x = 1 - (noiseLevel/2);
			imageStore(GroundImgHold, pixel_coords, sampled);
		}
	}
	else {
		if (action > 0 && action < 1 && digging > 0) {
			
			float Size = (4096.0 / 15.0 / 2);
			float dist = distance(pixel_coords, texCoords) / Size;

			float noiseLevel = noise(vec2(pixel_coords.x + 5812, pixel_coords.y + 321) / 200);
			noiseLevel /= 1;
			noiseLevel += 1;
			noiseLevel /= 2;

			float noiseLevel2 = noise(vec2(pixel_coords.x + 5812, pixel_coords.y + 321) / 50);
			noiseLevel2 /= 1;
			noiseLevel2 += 1;
			noiseLevel2 /= 2;

			float arcs = acos(dist + (noiseLevel2 /4.0));
			float distBetweenCentre = distance(sampled.x, middleSampled.x);
			arcs = sin(arcs);// *Size;
			//arcs -= (Size - 1);
			if (dist < Size && !isnan(arcs) && distBetweenCentre < 0.2) {

				//noiseLevel *= (distBetweenCentre * 2);
				//noiseLevel = clamp(noiseLevel, 0, 1);
				sampled.x = imageLoad(GroundImgHold, pixel_coords).x - (arcs * action *  ( 1 - distBetweenCentre ) * subBy * noiseLevel);
				sampled.x = clamp(sampled.x, 0, 1);
			}


		}
		else if (action >= 1 && digging > 0) {
			imageStore(GroundImgHold, pixel_coords, sampled);
		}
		if (action > 0 && digging <= 0) {
			imageStore(GroundImgHold, pixel_coords, sampled);
		}
	}




	imageStore(GroundImg, pixel_coords, sampled);
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