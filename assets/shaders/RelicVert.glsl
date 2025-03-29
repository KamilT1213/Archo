#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_UV;

out vec2 texCoords;


layout(std140, binding = 1) uniform b_relicCamera2D
{
	uniform mat4 u_relicView2D;
	uniform mat4 u_relicProjection2D;
};


//uniform mat4 u_relicView2D;
//uniform mat4 u_relicProjection2D;

uniform mat4 u_model;


void main()
{
	texCoords = a_UV;
	gl_Position = u_relicProjection2D * u_relicView2D * (u_model * vec4(a_vertexPosition,1.0));

}