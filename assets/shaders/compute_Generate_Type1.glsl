#version 460 core
layout(local_size_x = 32, local_size_y = 32) in;
layout(binding = 0, rgba16f) uniform image2D ImgIn;
layout(binding = 1, rgba16f) uniform image2D ImgOut;

uniform float Type;
uniform float Seed;
uniform float Depth;
uniform vec2 Size;

float hash(vec2 p);
vec2 randvec2(vec2 p);
vec2 fade(vec2 t);
float perlin(vec2 p, vec2 fp);
vec3 voronoi(vec2 p, vec2 fp);
mat2 rotate2d(float _angle);

float pi = 3.1415926;
float factor;


void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	vec2 center_pixel_coords = pixel_coords - (Size / 2);
	//ivec2 texCoords = ivec2(4096 / 2 * MousePos.x, 4096 / 2 * MousePos.y);


	float pixels = 70;
	vec2 fraction = Size / pixels;
	vec2 clampCoords = floor(center_pixel_coords / fraction);
	vec2 fractCoords = mod(center_pixel_coords, fraction) / fraction;

	if (Type == 0) {
		vec2 dir = vec2(0,1) * rotate2d(pi * Seed * 5935);
		vec2 dir2 = vec2(0, 1) * rotate2d(pi * Seed * 3365);
		clampCoords *= rotate2d(pi * Seed * 6215);
		float s = pixels / 3.0;
		s = (s + sin(Seed * 1542));
		float a = -((sqrt(pow(clampCoords.x,2) + pow(clampCoords.y,2))) - s - (s / 5.0));
		float b = (sqrt(pow(clampCoords.x,2) + pow(clampCoords.y,2))) - s + (s / 5.0);

		a = clamp(a, 0, 1);
		b = clamp(b,0,1);

		vec4 c = vec4(a * b);
		if(abs(clampCoords.y) < (s / 10.0)) c = vec4(1) * a;
		if(dot(dir,normalize(clampCoords)) > 0.8) c = vec4(0);
		if(dot(dir2,normalize(clampCoords)) > 0.8) c = vec4(0);
		imageStore(ImgOut, pixel_coords, vec4(c.rgb,step(0.95,c.a)));
		//imageStore(ImgIn, pixel_coords, vec4(1.0));
	}
	if (Type == 1) {

		vec3 outV = voronoi(clampCoords, fractCoords);
		vec4 sampled = imageLoad(ImgIn, ivec2(floor((outV.xy + (pixels / 2.0)) * fraction)));
		imageStore(ImgOut, pixel_coords, sampled);
	}
	if (Type == 2) {

		int target = 150;
		bool reached = false;
		for (int d = target; d > 0; d--) {
			for (int i = 0; i <= 128; i++) {
				float a = float(i) / 128.0;
				a *= pi * 2;
				vec2 dir = vec2(cos(a), sin(a)) * (d / 2);
				float sam = imageLoad(ImgIn, ivec2(pixel_coords + dir)).a;
				if (sam > 0.0) {
					reached = true;
					break;
				}
			}
			if (reached) {
				break;
			}
		}
		
		if (reached) {
			imageStore(ImgOut, pixel_coords, vec4(1.0));
		}

	}
	if (Type == 3) {

		float perFactor = 25 * (Depth);
		vec2 coords2 = floor(center_pixel_coords / (Size / perFactor));
		vec2 fcoords2 = mod(center_pixel_coords, (Size / perFactor)) / (Size / perFactor);


		vec4 sampled = imageLoad(ImgIn, pixel_coords);
		sampled.y -= (perlin(coords2 + Seed, fcoords2 - 0.5) + 1) / 16;
		sampled.y = step(0.01, sampled.y);
		sampled.x = 1 - sampled.y;
		sampled.a = step(0.01, sampled.y);
		imageStore(ImgOut, pixel_coords, vec4(vec3(1.0) * sampled.a,sampled.a));
	}
	if (Type == 4){
		float dist = 1024.0;

		float alpha = imageLoad(ImgIn, pixel_coords).a;

		if(alpha > 0){
			for(int d = 1; d < 256.0 * 2.0; d++){
				for(int i = 0; i <= 256; i++){
					float a = float(i) / 256;
					a *= pi * 2;
					vec2 dir = vec2(cos(a),sin(a)) * (d / 2);
					float sam = imageLoad(ImgIn, ivec2(pixel_coords + dir)).a;
					if(sam <= 0.1){
						if((d / 2) < dist){
							dist = d / 2;
						}
					}
				}
				if(dist < 256.0){
					break;
				}
			}

			dist /= 256.0;
			//dist = distance(vec2(0),fade(vec2(dist)));
			dist = smoothstep(0.0,1.0,dist);
			imageStore(ImgOut, pixel_coords, vec4(1 - dist, dist, 1.0 ,1.0));
		}
		else {
			imageStore(ImgOut, pixel_coords, vec4(0.0, 0.0, 0.0, 0.0));
		}


	}
	
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
