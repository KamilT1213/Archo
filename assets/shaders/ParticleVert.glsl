#version 460 core

//out vec2 texCoords;
out flat int ID;

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
	//texCoords = a_UV;
	ID = gl_VertexID;
	gl_PointSize = particles[gl_VertexID].size;
	gl_Position = vec4((particles[gl_VertexID].position - 0.5) * 2.0,(particles[gl_VertexID].depth - 0.5) * 2.0,1.0);
}