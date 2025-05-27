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

uniform float dt;
uniform float allTime;
float pi = 3.1415;


void main()
{
	uint Task = gl_WorkGroupID.x;
	uint Particle = gl_GlobalInvocationID.x;

	if(tasks[Task].Mode == 0){
		particles[Particle].colour.a = 0.0;
		particles[Particle].depth = 1.0;
	}
	else if( tasks[Task].Mode == 1){
		particles[Particle].size = 16;
		particles[Particle].colour = vec4(Particle / 128,1,1,1);
		particles[Particle].depth = 0.5;
		if(particles[Particle].position.y > 1) particles[Particle].position.y = 0;
		particles[Particle].position.y += (dt / 10.0) * ((mod(Particle,16) + 1)/16);
		particles[Particle].depth = (sin(particles[Particle].position.y * 3.1415 * 10 * ((mod(Particle, 16) + 1) / 16)) + 1)/2.0;
		particles[Particle].position.x = (Task / 8.0);
	}
	else if (tasks[Task].Mode == 2) {
		float offsets = ((mod(Particle, 16) + 1) / 16);
		particles[Particle].size = 8;
		particles[Particle].colour = vec4(Particle / 128, 1, 1, 1);
		particles[Particle].depth = 0.0;

		particles[Particle].position.y = tasks[Task].target.y + (sin((allTime + offsets ) * pi * 2 ) * tasks[Task].factor);
		particles[Particle].position.x = tasks[Task].target.x + (cos((allTime + offsets ) * pi * 2 ) * tasks[Task].factor);
	}

}
