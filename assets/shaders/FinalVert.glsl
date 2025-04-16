#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_UV;

out vec2 texCoords;


layout(std140, binding = 3) uniform b_finalcamera2D
{
	uniform mat4 u_finalview2D;
	uniform mat4 u_finalprojection2D;
};

uniform mat4 u_model;


void main()
{
	texCoords = a_UV;
	gl_Position = u_finalprojection2D * u_finalview2D * (u_model * vec4(a_vertexPosition,1.0));

}