#version 460 core
layout(local_size_x = 16) in;

struct ParticleData{
	vec4 colour;
	vec2 position;
	vec2 direction;
	float size;
	float speed;
	float lifespan;
	float depth;
};

struct ParticleBehaviour{
	vec2 target;
	float factor;
	int Mode;
};

layout(std430, binding = 6) buffer Data
{
	ParticleData particles[];
};

layout(std430, binding = 7) buffer Behaviour
{
	ParticleBehaviour tasks[];
};

uniform int TaskSize = 16;
uniform float dt;
uniform float allTime;
float pi = 3.1415;

float hash(vec2 p);

void main()
{
	uint Task = gl_WorkGroupID.x;
	uint Particle = gl_GlobalInvocationID.x;

	particles[Particle].position += particles[Particle].direction * dt * particles[Particle].speed;
	particles[Particle].lifespan -= dt;

	if(tasks[Task].Mode == 0){
		if (particles[Particle].lifespan <= 0) {
			particles[Particle].colour.a = 0.0;
			particles[Particle].depth = 1.0;
			particles[Particle].position = vec2(0.5);
			particles[Particle].direction = vec2(0.0);
			particles[Particle].size = 0.0;
			particles[Particle].speed = 0.0;
			particles[Particle].lifespan = 0.0;
		}
	}
	else if( tasks[Task].Mode == 1){

		if (particles[Particle].lifespan <= 0 && hash(vec2(((mod(Particle, TaskSize) + 1) / TaskSize) * mod(allTime, 1.25), dt)) > 0.99) {
			float offset1 = ((mod(Particle, TaskSize) + 1) / TaskSize) * mod(allTime, 3.51);
			float offset2 = (floor(mod(Particle, 2) - 0.5) / 2) * mod(allTime, 5.27);
			float rand = hash(vec2(offset1, offset2));
			float rand2 = hash(vec2(allTime, dt));
			float rand3 = hash(vec2(offset1, dt));
			particles[Particle].size = 4;
			particles[Particle].colour = vec4(0.3,0.3,0.3,0.7);
			particles[Particle].depth = 0;
			particles[Particle].direction = vec2(sin((allTime + rand) * pi * 2), cos((allTime + rand) * pi * 2));
			particles[Particle].position = tasks[Task].target;
			particles[Particle].speed = tasks[Task].factor * ( 0.5 + (0.5 * rand2)) * 1;
			particles[Particle].lifespan = tasks[Task].factor * (0.9 + (0.1 * rand3));
		}
		//particles[Particle].depth = (-abs((2 * particles[Particle].lifespan) - 1)) + 1;
	}
	else if (tasks[Task].Mode == 2) {
		float offsets = ((mod(Particle, TaskSize) + 1) / TaskSize);
		float depthOffset = floor(mod(Particle,2)-0.5)/2;
		particles[Particle].size = 8;
		particles[Particle].colour = vec4(Particle / 128, 1, 1, 1);
		particles[Particle].depth = 0;

		float tY = tasks[Task].target.y + (sin((allTime + offsets) * pi * 2) * tasks[Task].factor);
		float tX = tasks[Task].target.x + (cos((allTime + offsets) * pi * 2) * tasks[Task].factor);
		vec2 targetPos = vec2(tY, tX);
		targetPos = normalize(targetPos - particles[Particle].position);
		particles[Particle].direction = targetPos;
		particles[Particle].speed = clamp(distance(targetPos, particles[Particle].position), 0, 1);

	}
	else if (tasks[Task].Mode == 3) {

		float offsets = ((mod(Particle, TaskSize) + 1) / TaskSize);

		if (tasks[Task].factor > 2.5) {
			float tY = 0.5 + sin((allTime + offsets) * pi * 2);
			float tX = 0.5 + cos((allTime + offsets) * pi * 2);

			particles[Particle].size = 4;
			particles[Particle].colour = vec4(Particle / 128, 1, 1, 0.8);
			particles[Particle].depth = 0;

			particles[Particle].lifespan = 3;
			particles[Particle].position = vec2(tX, tY);


		}
		else if (tasks[Task].factor > 0.0) {
			
			float depthOffset = floor(mod(Particle, 2) - 0.5) / 2;


			float tY = tasks[Task].target.y + (sin((allTime + offsets) * pi * 2) * 0.02);
			float tX = tasks[Task].target.x + (cos((allTime + offsets) * pi * 2) * 0.02);

			vec2 targetPos = vec2(tX, tY);
			targetPos = normalize(targetPos - particles[Particle].position);
			particles[Particle].direction = targetPos;
			particles[Particle].speed = clamp(distance(targetPos, particles[Particle].position), 0.0, 1.0);
		}
	}

}

float hash(vec2 p) {
	p = fract(p * vec2(123.34, 456.21));
	p += dot(p, p + 78.233 + allTime);
	return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}
