#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_Texture;
uniform int u_index = -1;
uniform float Hovered;
uniform float Pressed;

struct Relic {
	int Quantity;
	int texture;
	int xOffset;
	int yOffset;
};

layout(std430, binding = 4) buffer ssbo
{
	Relic relics[];
};

float pi = 3.1415;

vec4 colPick(float a);


void main() {
	vec4 col = vec4(0.0);

	if (u_index >= 0) {


		float gridSize = 1;
		float gridThickness = 0.05;

		vec2 flippedCoords = vec2(texCoords.x, 1 - texCoords.y);
		vec2 localCoords = (flippedCoords + vec2(relics[u_index].xOffset, relics[u_index].yOffset)) / 8;
		col = texture((u_Texture), localCoords);

		int counter = 0;
		int progress = 0;
		float rotator = 0;
		for (int i = 0; i < 8; i++) {
			counter += int(pow(i + 1, 2));
			if (relics[u_index].Quantity <= counter) {
				progress = i;
				rotator = float(relics[u_index].Quantity - (counter - pow(i + 1, 2))) / pow(i + 1, 2.0);
				break;
			}
		}



		//vec2 local = texCoords - 0.5;
		//float RfromM = length(local);
		//if (RfromM < 0.5 && RfromM > 0.45 && atan(-local.x, -local.y) < (rotator * pi * 2) - pi) {
		//	col.xyz = vec3(colPick(progress + 0.5).xyz);
		//	col.a = 1.0;
		//}

		//col.xyz = vec3(float(mod(normed.y * gridSize, 1.0) > 0.98f) + float(mod(normed.x * gridSize, 1.0) > 0.98f) + float(mod(normed.x * gridSize,1.0) < 0.02f) + float(mod(normed.y * gridSize, 1.0) < 0.02f));
		if (relics[u_index].Quantity <= 0) discard;


		col.xyz *= 1 - (Hovered * 0.5);
		col.xyz *= 1 - (Pressed * 0.5);

	
		//colour = vec4(1);
	}
	else {
		//col = vec4(1, 0, 1, 1);

	}


	vec2 local = texCoords - 0.5;
	local = abs(local);
	float h = 16;
	float a = (pow(local.x, h) + pow(local.y, h)) - pow(0.5, h);
	float b = (pow(local.x, h) + pow(local.y, h)) - pow(0.5 - 0.05, h);

	if (a * b < 0) {
		col = vec4(1);
	}

	col.xyz *= 1 - (Hovered * 0.5);
	col.xyz *= 1 - (Pressed * 0.5);

	//if (col.a <= 0) discard;
	//col.xy = vtexCoords - 0.5);
	//col.a = 1.0;
	colour = col;

}
vec4 colPick(float a) {

	vec4 colour;

	if (a < 1) {
		colour = vec4(vec3(139 / 255.0, 143 / 255.0, 140 / 255.0), 1.0);
	}
	else if (a <= 2) {
		colour = vec4(vec3(48 / 255.0, 186 / 255.0, 83 / 255.0), 1.0);
	}
	else if (a <= 3) {
		colour = vec4(vec3(32 / 255.0, 104 / 255.0, 176 / 255.0), 1.0);
	}
	else if (a <= 4) {
		colour = vec4(vec3(123 / 255.0, 32 / 255.0, 176 / 255.0), 1.0);
	}
	else if (a <= 5) {
		colour = vec4(vec3(255 / 255.0, 153 / 255.0, 1 / 255.0), 1.0);
	}
	else if (a <= 6) {
		colour = vec4(vec3(255 / 255.0, 1 / 255.0, 1 / 255.0), 1.0);
	}

	return colour;
};
