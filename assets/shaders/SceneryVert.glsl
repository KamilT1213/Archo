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


void main()
{
	texCoords = a_UV;
	gl_Position = u_sceneryProjection2D * u_sceneryView2D * (u_model * vec4(a_vertexPosition,1.0));

}