#version 460 core
layout(local_size_x = 32, local_size_y = 32) in;
layout(binding = 0, rgba16f) uniform image2D ImgIn;
layout(binding = 1, rgba16f) uniform image2D ImgOut;

uniform float Type;
uniform float Seed;
uniform vec2 Size;

float hash(vec2 p);
vec2 randvec2(vec2 p);
vec2 fade(vec2 t);
float perlin(vec2 p, vec2 fp);
vec3 voronoi(vec2 p, vec2 fp);

float pi = 3.1415926;
float factor;


void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	vec2 center_pixel_coords = pixel_coords - (Size / 2);
	//ivec2 texCoords = ivec2(4096 / 2 * MousePos.x, 4096 / 2 * MousePos.y);


	float pixels = 30;
	vec2 fraction = Size / pixels;
	vec2 clampCoords = floor(center_pixel_coords / fraction);
	vec2 fractCoords = mod(center_pixel_coords, fraction) / fraction;

	if (Type == 0) {
		
		float s = pixels / 3.0;
		float a = -((sqrt(pow(clampCoords.x,2) + pow(clampCoords.y,2))) - s - (s / 5.0));
		float b = (sqrt(pow(clampCoords.x,2) + pow(clampCoords.y,2))) - s + (s / 5.0);

		a = clamp(a,0,1);
		b = clamp(b,0,1);

		vec4 c = vec4(a * b);
		if(abs(clampCoords.y) < (s / 10.0)) c = vec4(1) * a;
		imageStore(ImgIn, pixel_coords, vec4(c.rgb,step(0.95,c.a)));
	}
	if (Type == 1) {

		vec3 outV = voronoi(clampCoords, fractCoords);
		vec4 sampled = imageLoad(ImgIn, ivec2(floor((outV.xy + (pixels / 2.0)) * fraction)));
		imageStore(ImgIn, pixel_coords, sampled);
	}
	if (Type == 2) {

		vec3 total;
		float size = 25;

		int halfs = int(floor(size / 2));

		for(int x = -halfs; x <= halfs; x++){
			for(int y = -halfs; y <= halfs; y++){
				total += imageLoad(ImgIn, pixel_coords.xy + ivec2(x,y)).xyz * (1.0 / pow(size,2));
			}
		}

		//total = step(0.01,total);

		imageStore(ImgIn, pixel_coords, vec4(total, step(0.01,distance(vec3(0),total))) );

	}
	if (Type == 3) {
		vec4 sampled = imageLoad(ImgIn, pixel_coords);
		sampled.xyz *= (perlin(clampCoords + Seed, fractCoords - 0.5) + 1.0);
		imageStore(ImgIn, pixel_coords, sampled);
	}
	if (Type == 4){
		float dist = 100.0;

		float alpha = imageLoad(ImgIn, pixel_coords).a;

		if(alpha > 0){
			for(int d = 1; d < 200; d++){
				for(int i = 0; i <= 128; i++){
					float a = float(i) / 128.0;
					a *= pi * 2;
					vec2 dir = vec2(cos(a),sin(a)) * (d / 2);
					float sam = imageLoad(ImgIn, ivec2(pixel_coords + dir)).a;
					if(sam <= 0.1){
						if((d / 2) < dist){
							dist = d / 2;
						}
					}
				}
				if(dist < 100.0){
					break;
				}
			}

			dist /= 100;
			//dist = distance(vec2(0),fade(vec2(dist)));
			dist = smoothstep(0.0,0.5,dist);
			imageStore(ImgIn, pixel_coords, vec4(vec3(dist),1.0));
		}


	}
	

	//vec3 outV = voronoi(clampCoords, fractCoords);
	//float p = (perlin(clampCoords, fractCoords - 0.5) + 1.0) / 2.0;

	//vec3 c = vec3(hash(outV.xy),hash(outV.xy + 1202.0), hash(outV.xy - 8882.0)) * 1.5;

	//imageStore(ImgOut, pixel_coords, vec4( c2 * vec3( (1.0 - outV.z)) , 1));

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
