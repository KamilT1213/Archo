#version 460 core
layout(vertices = 3) out;


in vec3 vs_posInWS[];
in vec3 vs_centreInWS[];


out vec3 tcs_posInWS[];
out vec3 tcs_centreInWS[];

layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

float GetTessLevel(float Distance0, float Distance1);

void main()
{
	float eyeToVertexDist0 = distance(u_viewPos, vs_posInWS[0]);
	float eyeToVertexDist1 = distance(u_viewPos, vs_posInWS[1]);
	float eyeToVertexDist2 = distance(u_viewPos, vs_posInWS[2]);

	if (gl_InvocationID == 0) {
		gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2);
		gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0);
		gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1);
		gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	}

	tcs_posInWS[gl_InvocationID] = vs_posInWS[gl_InvocationID];
	tcs_centreInWS[gl_InvocationID] = vs_centreInWS[gl_InvocationID];
}

float GetTessLevel(float Distance0, float Distance1) {
	float AvgDistance = (Distance0 + Distance1) / 2.0;
	return 25 * exp(-(AvgDistance - 100.0) / 100.0);
}
