#version 460 core
layout(triangles, equal_spacing, ccw) in;

in vec3 tcs_fragmentPos[];
in vec2 tcs_texCoord[];
in vec3 tcs_vertPos[];
uniform sampler2D u_heightMap;

out vec3 tes_fragmentPos;
out vec2 tes_texCoord;



layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);

void main()
{
	vec3 fragmentPos = interpolate3D(tcs_vertPos[0], tcs_vertPos[1], tcs_vertPos[2]);
	tes_texCoord = interpolate2D(tcs_texCoord[0], tcs_texCoord[1], tcs_texCoord[2]);
	vec3 Displacememt = texture(u_heightMap, tes_texCoord).rgb - vec3(0.5);

	fragmentPos += 50 * Displacememt * 0.5;

	gl_Position = u_projection * u_view * vec4(fragmentPos, 1.0);
	tes_fragmentPos = interpolate3D(tcs_fragmentPos[0], tcs_fragmentPos[1], tcs_fragmentPos[2]);
	tes_fragmentPos += 50 * Displacememt * 0.5;
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {
	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}