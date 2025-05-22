#version 460 core
layout(local_size_x = 32, local_size_y = 32) in;
layout(binding = 0, rgba16f) uniform image2D VisibleLayer;
layout(binding = 1, rgba16f) uniform image2D HiddenLayer;
layout(binding = 2, rgba8) uniform image2D BrushMask;

uniform vec2 Size;
//uniform vec2 DigPos;

//uniform vec4 DigStyle;

uniform float Factor;
uniform float Mode;

uniform int DigSpotTotal;

float hash(vec2 p);
vec2 randvec2(vec2 p);
vec2 fade(vec2 t);
float perlin(vec2 p, vec2 fp);
vec3 voronoi(vec2 p, vec2 fp);
mat2 rotate2d(float _angle);


float Seed = 0.51765;
float pi = 3.1415926;

struct DiggingSpot {
	vec4 DigInfo; //Position: x, y | Radii | Depth
	float DigProgression;// Progession of digging
	int DigMask;// Pattern to use when digging
	float rotation;
	int a;// Reserved
};

layout(std430, binding = 5) buffer ssbo
{
	DiggingSpot digspots[];
};


void main()
{
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

	if(Mode == 0.0){
		vec4 Visible = imageLoad(VisibleLayer,coords);
		imageStore(HiddenLayer, coords, Visible);
	}
	else if(Mode == 1.0){
		if(Factor > 0.0){



			float sumRemoval = 0;
			float currentFactor = 0;

			for (int i = 0; i < DigSpotTotal; i++) {

				vec2 DigPos = digspots[i].DigInfo.xy;

				vec2 MaskCoords = (vec2(floor(mod(float(digspots[i].DigMask) + 0.5, 4.0)), floor(digspots[i].DigMask / 4)) * 256);

				Seed = DigPos.x / 2134.041;
				Seed = DigPos.y / 8715.230;
				Seed = mod(Seed, 1.0);

				float radii = Size.x / digspots[i].DigInfo.z;
				float dist = distance(coords, DigPos * Size) / radii;

				vec2 CoordOnMask = ((((coords - (DigPos * Size)) * rotate2d(digspots[i].rotation)) + radii) / (radii * 2)) * 256;
				MaskCoords += CoordOnMask;

				vec2 NoiseS1 = vec2(coords) / 3.125;

				float noiseLevel = perlin(floor(NoiseS1), fract(NoiseS1) - 0.5);
				noiseLevel += 1;
				noiseLevel /= 2;

				vec2 NoiseS2 = vec2(coords + (DigPos * 421)) / 1.25;

				float noiseLevel2 = perlin(floor(NoiseS2), fract(NoiseS2) - 0.5);
				noiseLevel2 += 1;
				noiseLevel2 /= 2;

				vec4 sampled = imageLoad(VisibleLayer, coords);
				vec4 middleSampled = imageLoad(VisibleLayer, ivec2(DigPos * Size));

				float arcs = acos(dist + (noiseLevel2 / 4.0));
				float distBetweenCentre = distance(sampled.x + sampled.y, middleSampled.x + middleSampled.y);
				arcs = sin(arcs);// *Size;

				if (dist < 1.0 && !isnan(arcs) && distBetweenCentre < 0.2) {

					
					float removal = (arcs * (1 - dist) * digspots[i].DigInfo.w * (1 - (noiseLevel * dist))) * imageLoad(BrushMask,ivec2(MaskCoords)).y;
					if (removal > sumRemoval) {
						sumRemoval = removal;
						currentFactor = digspots[i].DigProgression;
					}

				}
			}

			if (sumRemoval > 0) {
				vec4 Hidden = imageLoad(HiddenLayer, coords);

				Hidden.y = mix(Hidden.y - sumRemoval, Hidden.y, currentFactor);
				Hidden.z = mix(Hidden.z + sumRemoval, Hidden.z, currentFactor);

				imageStore(VisibleLayer, coords, Hidden);
			}
		}
		if(Factor <= 0.0 || Factor >= 1.0){
			vec4 Visible = imageLoad(VisibleLayer,coords);
			imageStore(HiddenLayer, coords, Visible);
		}

	}
	else if(Mode == 1.5){
		vec4 Visible = imageLoad(VisibleLayer,coords);
		imageStore(HiddenLayer, coords, Visible);
	}
	else if(Mode == 2.0){
		vec4 Visible = imageLoad(VisibleLayer,coords);
		Visible.y = clamp(Visible.y,0.0,Factor);
		imageStore(VisibleLayer, coords, Visible);
	}

	vec4 Visible = imageLoad(VisibleLayer,coords);
	vec4 Hidden = imageLoad(HiddenLayer,coords);

	imageStore(VisibleLayer, coords, Visible);
	imageStore(HiddenLayer, coords, Hidden);
}

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 78.233 + Seed);	
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 randvec2(vec2 p){
	float r = hash(p) * pi * 47283.0;
	return vec2(cos(r),sin(r));
}

vec2 fade(vec2 t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float perlin(vec2 p, vec2 fp){
	float a = dot(randvec2(p + vec2(0,0)),(fp + vec2(0.5,0.5)));
	float b = dot(randvec2(p + vec2(0,1)),(fp + vec2(0.5,-0.5)));
	float c = dot(randvec2(p + vec2(1,0)),(fp + vec2(-0.5,0.5)));
	float d = dot(randvec2(p + vec2(1,1)),(fp - vec2(0.5,0.5)));

	vec2 t = fade(fp + 0.5);

	float r1 = mix(a,b,t.y);
	float r2 = mix(c,d,t.y);

	float r3 = mix(r1,r2, t.x);

	return r3;
}

vec3 voronoi(vec2 p, vec2 fp){
	int s = 15;
	int s2 = 7;
	vec2 points[225];

	for(int x = -s2; x <= s2; x++){
		for(int y = -s2; y <= s2; y++){
			int index = (x + s2) + ((y + s2) * s);
			points[index] = vec2(x,y);
		}
	}

	vec2 closest;
	float dist = 9999.0;
	vec2 point;

	for(int i = 0; i < 225; i++){
		point = (points[i]) + (randvec2(p + points[i]) * 1.0);
		float tdist = distance(fp,point);
		if(tdist < dist){
			dist = tdist;
			closest = p + points[i];
		}
	}

	return vec3(closest,dist * 0.75);
}

mat2 rotate2d(float _angle) {
	return mat2(cos(_angle), -sin(_angle),
		sin(_angle), cos(_angle));
}
