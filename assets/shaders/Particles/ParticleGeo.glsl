#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

uniform sampler2D u_heightMap;

in vec2 vs_texCoords[];

out vec2 texCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	mat4 VP = u_projection * u_view;
	vec3 pos = gl_in[0].gl_Position.xyz;
	
	vec3 toCamera = normalize(u_viewPos - pos);
	Normal = toCamera;
	float yOffset = ((abs(texture(u_heightMap, vs_texCoords[0]).y - 0.5) * 25));
	float o = clamp((-(pow(((gl_in[0].gl_Position.w / yOffset) * 2) - 1, 2))) + 1, 0, 1);

	vec3 up = vec3(0, 1, 0);
	vec3 right = normalize(cross(toCamera, up));
	up = cross(-right, toCamera);

	up *= 0.25 * o;
	right *= 0.25 * o;
	if (distance(vec3(0, 0, 0), pos) > 0.0) {
		//Bottom-left corner (-right, -up)
		gl_Position = VP * vec4(pos - right - up, 1.0);
		texCoord = vec2(0, 0);
		FragPos = (pos - right - up);
		EmitVertex();

		//Top-left corner (-right, +up)
		gl_Position = VP * vec4(pos - right + up, 1.0);
		texCoord = vec2(0, 1.0);
		FragPos = (pos - right + up);
		EmitVertex();

		//Bottom-right corner (+right, -up)
		gl_Position = VP * vec4(pos + right - up, 1.0);
		texCoord = vec2(1.0, 0);
		FragPos = (pos - right + up);
		EmitVertex();

		//Top-right corner (+right, +up)
		gl_Position = VP * vec4(pos + right + up, 1.0);
		texCoord = vec2(1.0, 1.0);
		FragPos = (pos - right + up);
		EmitVertex();

	}
}
