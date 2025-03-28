#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tes_posInWS[];

layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};



void main()
{

	for (int i = 0; i < 3; i++) {


		gl_Position = u_projection * u_view * vec4(tes_posInWS[i], 1.0);

		EmitVertex();

	}

	EndPrimitive();

}
