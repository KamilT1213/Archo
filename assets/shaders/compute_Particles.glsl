#version 460 core
layout(local_size_x = 128) in;

struct Particle {
	vec4 position;
	vec2 UV;
};

layout(std430, binding = 0) buffer vertex0
{
	Particle startingPoints[];
};

layout(std430, binding = 1) buffer vertex1
{
	Particle particles[];
};

layout(binding = 0, rgba16) uniform image2D outputImg;

uniform float dt;

float speed = 2.0;

void main()
{
	vec4 startingPoint = startingPoints[gl_GlobalInvocationID.x].position;
	vec4 particlePos = particles[gl_GlobalInvocationID.x].position;
	vec2 UVin = startingPoints[gl_GlobalInvocationID.x].UV;
	vec2 UVout = particles[gl_GlobalInvocationID.x].UV;

	ivec2 p = ivec2((UVout * 4096).x, (UVout * 4096).y);
	ivec2 p2 = ivec2((UVin * 4096).x, (UVin * 4096).y);
	float yOffset = (abs(imageLoad(outputImg, p).y - 0.5)) * 25;
	float yOffset2 = (abs(imageLoad(outputImg, p2).y - 0.5)) * 25;

	if (distance(startingPoint.xyz, vec3(0)) > 0.0 && particlePos.a == 0.0) {// && yOffset2 > 0) {
		particlePos = startingPoint;
		UVout = UVin;
		particlePos.xyz += vec3(0, 1, 0);

		particlePos.a = yOffset;

	}
	else if (particlePos.a >= 1.0 ) {

		
		particlePos.xyz -= vec3(0,dt * speed, 0);
		particlePos.a -= (dt) * speed;

	}
	else if (particlePos.a <= 1.0) {
		particlePos = vec4(0);
		UVout = vec2(0);
	}

	particles[gl_GlobalInvocationID.x].position = particlePos;
	particles[gl_GlobalInvocationID.x].UV = UVout;
}
