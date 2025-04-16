#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_MainMenuIn;
uniform sampler2D u_InGameIn;
uniform sampler2D u_PausedIn;

uniform float u_State;

void main()
{
    vec4 total = vec4(0, 0, 0, 1);
    if (u_State == 0) {
        total = texture(u_MainMenuIn, texCoords);
    }
    else if (u_State == 1) {
        total = texture(u_InGameIn, texCoords);
    }
    else if (u_State == 2) {
        total = texture(u_PausedIn, texCoords);
    }
    colour = total;// vec4(vec3(rgb2hsv(Samples[4].rgb).z), 1); //vec4(vec3(Edge()), 1);//

}
