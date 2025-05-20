#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_ButtonTexture;
uniform float ButtonID;
uniform float Hovered;
uniform float Pressed;

void main() {

	//colour = vec4(1);
	vec4 col = texture(u_ButtonTexture, (texCoords * 1.2) - 0.1);
	
	if (col.a <= 0.1) {
		if ((max(abs(texCoords - 0.5).x, abs(texCoords - 0.5).y)) < 0.45) col.a = 1.0;
		col.xyz = vec3(0.3, 0.3, 0.6);
		col.xyz *= 1 - (Hovered * 0.1);
		col.xyz *= 1 - (Pressed * 0.1);
	}
	

	if (col.a <= 0.1) discard;
	colour = col;
	//colour = vec4(1);
}