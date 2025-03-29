#version 460 core
			
layout(location = 0) out vec4 colour;
layout(location = 1) out vec3 gFragPos;
layout(location = 2) out vec3 gNorm;
layout(location = 3) out vec4 gSpecMetallicRoughness;

in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D u_billboardImage;

layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

void main()
{

	vec4 Acolour = vec4((vec3(1,1,1) - texture(u_billboardImage, texCoord * 0.99).rbg) * vec3(149.0/255.0,104.0/255.0,49.0/255.0),texture(u_billboardImage, texCoord * 0.99).a);

	if (Acolour.a <= 0.1) discard;


	gFragPos = FragPos;

	gNorm = vec3(0,1,0);// Normal;

	gSpecMetallicRoughness = vec4(0.1,0.5,0.5,1.0);

	colour = vec4(Acolour.rgb, Acolour.a);
}