#version 460 core

layout(location = 0)out vec4 colour;

//in vec2 texCoords;

//uniform sampler2D u_Texture;

in flat int ID;

struct ParticleData{
	vec4 colour;
	vec2 position;
	vec2 direction;
	float size;
	float speed;
	float lifespan;
	float depth;
};

layout(std430, binding = 6) buffer ssbo
{
	ParticleData particles[];
};

void main()
{
    
    colour = particles[ID].colour;
    

}

