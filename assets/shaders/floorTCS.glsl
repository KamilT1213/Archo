#version 460 core
layout(vertices = 3) out;

in vec3 vs_fragmentPos[];
in vec2 vs_texCoord[];
in vec3 vs_vertPos[];

out vec3 tcs_fragmentPos[];
out vec2 tcs_texCoord[];
out vec3 tcs_vertPos[];

uniform sampler2D u_heightMap;

layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

float GetTessLevel(float Distance0, float Distance1);
float GetDisplacedTessLevel(float Distance0, float Distance1);

void main()
{
	float tesslevel = 4.0;

	vec3 Displacement0 = (texture(u_heightMap, vs_texCoord[0]).rgb - vec3(0.5)) * 25.0;
	vec3 Displacement1 = (texture(u_heightMap, vs_texCoord[1]).rgb - vec3(0.5)) * 25.0;
	vec3 Displacement2 = (texture(u_heightMap, vs_texCoord[2]).rgb - vec3(0.5)) * 25.0;

	float DisplacementDifference0 = distance(vs_vertPos[0] + Displacement0, vs_vertPos[1] + Displacement1);
	float DisplacementDifference1 = distance(vs_vertPos[1] + Displacement1, vs_vertPos[2] + Displacement2);
	float DisplacementDifference2 = distance(vs_vertPos[2] + Displacement2, vs_vertPos[0] + Displacement0);

	float DisplacementTotal0 = 1 + (abs(Displacement0.x) + abs(Displacement0.y)  + abs(Displacement0.z) );
	float DisplacementTotal1 = 1 + (abs(Displacement1.x) + abs(Displacement1.y)  + abs(Displacement1.z) );
	float DisplacementTotal2 = 1 + (abs(Displacement2.x) + abs(Displacement2.y)  + abs(Displacement2.z) );

	float eyeToVertexDist0 = distance(u_viewPos, vs_vertPos[0] + Displacement0);
	float eyeToVertexDist1 = distance(u_viewPos, vs_vertPos[1] + Displacement1);
	float eyeToVertexDist2 = distance(u_viewPos, vs_vertPos[2] + Displacement2);

	

	if (gl_InvocationID == 0) {
		gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2) +  GetDisplacedTessLevel(DisplacementDifference1, DisplacementDifference2);
		gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0) +  GetDisplacedTessLevel(DisplacementDifference2, DisplacementDifference0);
		gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1) +  GetDisplacedTessLevel(DisplacementDifference0, DisplacementDifference1);
		gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	}

	tcs_fragmentPos[gl_InvocationID] = vs_fragmentPos[gl_InvocationID];
	tcs_texCoord[gl_InvocationID] = vs_texCoord[gl_InvocationID];
	tcs_vertPos[gl_InvocationID] = vs_vertPos[gl_InvocationID];
	
}

float GetTessLevel(float Distance0, float Distance1) {
	float AvgDistance = (Distance0 + Distance1) / 2.0;
	return 10 * exp(-(AvgDistance - 15) / 15.0);
}

float GetDisplacedTessLevel(float Distance0, float Distance1) {
	float AvgDistance = (Distance0 + Distance1) / 2.0;
	return AvgDistance * 1;// 1 * exp(-(AvgDistance - 15) / 15.0);
}