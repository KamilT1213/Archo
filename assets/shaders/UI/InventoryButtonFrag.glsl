#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_ButtonTexture;
uniform float ButtonID;
uniform float Hovered;
uniform float Pressed;

void main() {

	//colour = vec4(1);
	vec4 col = texture(u_ButtonTexture, texCoords);
	if (col.a <= 0.1) discard;
	col.xyz *= 1 - (Hovered * 0.1);
	col.xyz *= 1 - (Pressed * 0.1);
	colour = col;
	//colour = vec4(1);
}