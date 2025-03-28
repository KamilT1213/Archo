#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

uniform mat4 u_model;


void main()
{
	vec3 fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
	gl_Position = u_projection * u_view * vec4(fragmentPos,1.0);
}