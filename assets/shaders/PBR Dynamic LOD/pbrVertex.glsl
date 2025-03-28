#version 460 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTan;

out vec2 vs_UV ;
out vec3 vs_posInWS ;
out vec3 vs_centreInWS;


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

uniform mat4 u_model;


void main()
{  
    vs_posInWS = (u_model*vec4(aPos,1.0)).xyz; 
	vs_centreInWS = (u_model * vec4(vec3(0), 1.0)).xyz;
    //gl_Position = u_projection*u_view*vec4(vs_posInWS,1.0);
    vs_UV = aUV ;
   
}