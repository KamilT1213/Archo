#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_normals;
layout(location = 2) in vec2 a_UV;

out vec2 texCoords;


layout(std140, binding = 4) uniform b_sceneryCamera2D
{
	uniform mat4 u_sceneryView2D;
	uniform mat4 u_sceneryProjection2D;
};

uniform mat4 u_model;
uniform float u_DugOut;

void main()
{
	texCoords = a_UV;
	vec3 pos = a_vertexPosition;
	pos.z = mix(pos.z,1.0, u_DugOut * 0.99);
	pos.z = clamp(pos.z, 0, 1);
	gl_Position = u_sceneryProjection2D * u_sceneryView2D * (u_model * vec4(pos,1.0));

}