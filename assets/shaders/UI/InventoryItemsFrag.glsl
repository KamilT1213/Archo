#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_Texture;
uniform vec2 u_GridSize;

uniform int u_index;
uniform int u_index2;
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
	//colour = vec4(1);
	float ratio = u_GridSize.x / u_GridSize.y;
	vec4 col = vec4(1.0);//texture(u_Texture, texCoords);#
	vec2 normed = texCoords * vec2(ratio, 1);
	//if (col.a <= 0.1) discard;
	//if (abs(normed.x - ratio) > (0.4 * ratio)) col.xyz = vec3(0.0);
	//if (abs(normed.y - 0.5) > 0.4) col.xyz = vec3(0.0);
	float gridSize = u_GridSize.y;
	float gridThickness = 0.05;


	int index = int(floor(floor((normed)*gridSize).x + (floor((1 - normed)*gridSize).y * (gridSize * ratio))));
	vec2 flippedCoords = vec2(normed.x, 1 - normed.y);
	vec2 localCoords = ((mod(flippedCoords * gridSize, 1.0) + vec2(relics[index].xOffset, relics[index].yOffset)) / 4) ;
	col = texture((u_Texture), localCoords);
	//col.xyz = colPick(index + 0.5).xyz * (step(length(mod(normed * gridSize, 1.0) - 0.5), 0.45));
	//col.xyz = vec3(index / 48.0);
	//if (relics[index].Quantity > 0) col.xyz = vec3(relics[index].Quantity / 100.0);

	int counter = 0;
	int progress = 0;
	float rotator = 0;
	for (int i = 0; i < 8; i++) {
		counter += int(pow(i + 1, 2));
		if (relics[index].Quantity <= counter) {
			progress = i ;
			rotator = float(relics[index].Quantity - (counter - pow(i + 1, 2)) ) / pow(i + 1, 2.0);
			break;
		}
	}

	

	vec2 local = mod(normed * gridSize, 1.0) -0.5;
	float RfromM = length(local);
	if (RfromM < 0.5 && RfromM > 0.45 && atan(-local.x, -local.y) < (rotator * pi * 2) - pi) {
		col.xyz = vec3(colPick(progress + 0.5).xyz);
		col.a = 1.0;
	}

	//col.xyz = vec3(float(mod(normed.y * gridSize, 1.0) > 0.98f) + float(mod(normed.x * gridSize, 1.0) > 0.98f) + float(mod(normed.x * gridSize,1.0) < 0.02f) + float(mod(normed.y * gridSize, 1.0) < 0.02f));



	if (u_index2 == index) {
		col.xyz *= 1 - (Hovered * 0.5);
		col.xyz *= 1 - (Pressed * 0.5);

	}
	if (u_index == index) {
		
		if (RfromM < 0.45 && RfromM > 0.40 && sin(atan(-local.x, -local.y) * 32) > 0) {
			col.xyz = vec3(1,1,1);
			col.a = 1.0;
		}
	}
	if (relics[index].Quantity <= 0 || col.a <= 0) discard;
	colour = col;
	//colour = vec4(1);
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
