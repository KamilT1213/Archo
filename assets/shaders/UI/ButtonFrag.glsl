#version 460 core

layout(location = 0)out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_ButtonTexture;
uniform float ButtonID;

void main() {

	//colour = vec4(1);
	colour = vec4(texCoords, ButtonID,1.0);
}