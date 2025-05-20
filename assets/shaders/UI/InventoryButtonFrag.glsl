#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_ButtonTexture;
uniform float ButtonID;
uniform float Hovered;
uniform float Pressed;

vec2 hash2(vec2 p);
float noise(in vec2 p);


void main() {

	//colour = vec4(1);
	vec4 col = texture(u_ButtonTexture, (texCoords * 1.2) - 0.1);
	
	if (col.a <= 0.1) {
		if ((max(abs(texCoords - 0.5).x, abs(texCoords - 0.5).y)) < 0.45) col.a = 1.0;
		col.xyz = vec3(0.3, 0.3, 0.6);
		col.xyz *= 1 - (Hovered * 0.1);
		col.xyz *= 1 - (Pressed * 0.1);
		float a = max(abs(texCoords.x - 0.5),abs(texCoords.y - 0.5));
		col.a *= step(0.4,0.8- a) + step(0.4,step(0.4,a) * smoothstep(0.0,1.0,mod(-a,0.05) * 20) * ((noise(texCoords * 30) + 1)/2.0) * 1);
		col.xyz = mix(vec3(0.5, 0.4, 0.1),vec3(0.5, 0.5, 0.1),((noise(texCoords * 30) + 1)/2.0));
		col.xyz = mix(col.xyz, vec3(1.0), ((noise(texCoords * 100 * vec2(1,4)) + 1)/4.0));
	}
	

	if (col.a <= 0.0) discard;
	colour = col;
	//colour = vec4(1);
}

vec2 hash2(vec2 p)
{
	p = vec2(dot(p, vec2(127.1, 311.7)),
		dot(p, vec2(269.5, 183.3)));

	return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise(in vec2 p)
{
	vec2 i = floor(p);
	vec2 f = fract(p);

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(dot(hash2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
		dot(hash2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
		mix(dot(hash2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
			dot(hash2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}