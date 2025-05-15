#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_Texture;

void main() {

	//colour = vec4(1);
	float ratio = 3.0 / 4.0;
	vec4 col = vec4(1.0);//texture(u_Texture, texCoords);#
	vec2 normed = texCoords * vec2(ratio, 1);
	//if (col.a <= 0.1) discard;
	//if (abs(normed.x - ratio) > (0.4 * ratio)) col.xyz = vec3(0.0);
	//if (abs(normed.y - 0.5) > 0.4) col.xyz = vec3(0.0);
	float gridSize = 8;
	float gridThickness = 0.05;
	col.xyz = vec3(step(length(mod(normed * gridSize, 1.0) - 0.5),0.45));
	//col.xyz = vec3(float(mod(normed.y * gridSize, 1.0) > 0.98f) + float(mod(normed.x * gridSize, 1.0) > 0.98f) + float(mod(normed.x * gridSize,1.0) < 0.02f) + float(mod(normed.y * gridSize, 1.0) < 0.02f));
	if (col.x <= 0) discard;
	colour = col;
	//colour = vec4(1);
}