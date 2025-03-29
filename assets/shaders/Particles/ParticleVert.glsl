#version 460 core

struct Particle {
	vec4 position;
	vec2 UV;
};

layout(std430, binding = 1) buffer particles
{
	Particle p[];
};

uniform mat4 u_model;

out vec2 vs_texCoords;
 
void main()
{
	//vs_pos = (u_model * vec4(p[gl_VertexID].position.xyz, 1.0)).xyz;
	gl_Position = u_model * p[gl_VertexID].position.xyzw;
	vs_texCoords = p[gl_VertexID].UV;
}