#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_background;
uniform sampler2D u_game;
uniform sampler2D u_buttons;
uniform vec2 u_mousePos;
uniform vec2 u_ScreenSize;

float sizeOfRing = 0.01;

void main()
{
    float ScreenPixelSize = 1.0 / min(u_ScreenSize.x, u_ScreenSize.y);
    float ScreenPixelRatio = 1.0 / (u_ScreenSize.x / u_ScreenSize.y);
    vec2 ratio = 1.0 / u_ScreenSize;

    vec4 col = texture(u_buttons, texCoords);

    if (col.a <= 0.95) {
        col = texture(u_game, ((floor(texCoords * (u_ScreenSize / 3.0)) + 0.5) / (u_ScreenSize / 3.0)));
    }

    if (col.a <= 0.1) {
        col = texture(u_background, texCoords);
    }

    vec2 localToMouse = (texCoords - (u_mousePos / u_ScreenSize));
    localToMouse.y *= ScreenPixelRatio;
    float RfromM = distance(localToMouse, vec2(0));
    if (RfromM < (sizeOfRing * 0.75)) {
        col = mix(col, vec4(vec3(1), 1), 0.4f);
    }

    colour = col;
    
}

