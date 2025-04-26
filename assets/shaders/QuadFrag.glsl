#version 460 core

layout(location = 0)out vec4 colour;
layout(location = 1)out vec4 data;

in vec2 texCoords;

uniform vec2 MousePos;
uniform vec2 DigPos;
uniform float Progress;
uniform float RelicFill;

uniform vec2 u_ScreenSize;
uniform sampler2D u_GroundDepthTexture;
uniform sampler2D u_RelicTexture;
uniform sampler2D u_RelicDataTexture;

uniform float u_flip;

float pi = 3.1415;
float sizeOfRing = 15;

vec2 hash2(vec2 p);
float noise(in vec2 p);
float tpTex(vec3 normal, vec3 pos, float factor);

float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 78.233 + 0.74234);	
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float ScreenPixelSize = 1.0/min(u_ScreenSize.x, u_ScreenSize.y);
    float groundDepth = texture(u_GroundDepthTexture, texCoords).r + texture(u_GroundDepthTexture, texCoords).g;
    vec4 colourout = texture(u_GroundDepthTexture, texCoords);
    float groundDepthAtDig = texture(u_GroundDepthTexture, DigPos).r;
    vec2 dir = normalize(vec2(1,-1));
    float groundDepthoff = texture(u_GroundDepthTexture, texCoords + (-dir * (1.0 / u_ScreenSize))).r;

    float n1 = texture(u_GroundDepthTexture, texCoords + (vec2(-1,0) * (1.0 / u_ScreenSize))).r;
    float n2 = texture(u_GroundDepthTexture, texCoords + (vec2(-1,0) * (1.0 / u_ScreenSize))).r;
    float n3 = texture(u_GroundDepthTexture, texCoords + (vec2(0,1) * (1.0 / u_ScreenSize))).r;
    float n4 = texture(u_GroundDepthTexture, texCoords + (vec2(0,-1) * (1.0 / u_ScreenSize))).r + texture(u_GroundDepthTexture, texCoords + (vec2(0,-1) * (1.0 / u_ScreenSize))).g;

    float dif1 = n1 - n2;
    float dif2 = n3 - n4;

    vec3 norm = normalize(vec3(dif1,1.0,dif2));
    float Dot = dot(norm, -normalize(vec3(dir.x, -1.0, dir.y)));

    float noiseDis  = tpTex(norm, vec3(texCoords.x, groundDepth, texCoords.y) , 200);
    noiseDis += tpTex(norm, vec3(texCoords.x, groundDepth, texCoords.y) , 100);
    noiseDis += tpTex(norm, vec3(texCoords.x, groundDepth, texCoords.y), 400);
    noiseDis /= 3;
    noiseDis += 1;
    noiseDis /= 10;


    colour = vec4(vec3(groundDepth),1.0);
    if(groundDepth > 0.5) colour = mix( vec4(0.302, 0.247, 0.302, 1.0),vec4(0.0, 1.0, 0.0, 1.0), (groundDepth * 2.0) - 1.0);
    if(groundDepth < 0.5) colour = mix( vec4(1.0, 0.0, 1.0, 1.0),vec4(0.302, 0.247, 0.302, 1.0), (groundDepth * 2.0));
    if(groundDepth < groundDepthoff && distance(groundDepth,groundDepthoff)  > 0.0001 || Dot - (1 - groundDepth) < 0.35){
        colour -= vec4(0.15, 0.15, 0.15, 0.0) * (1.0 * (1 - groundDepth));
    }

    colour += vec4(vec3(-noiseDis), 0.0);

    colour.xyz = ((floor(colour.xyz * 20) + 0.5) / 20);

    if(colourout.z < 0.01) colour.xyz += vec3(hash(floor(texCoords * 1024)) / 20.0);
    
    data = vec4(vec3(0.0),0.0);

    vec4 c = texture(u_RelicTexture,texCoords);
    vec4 d = texture(u_RelicDataTexture,texCoords);
    if(c.a >= 1 && ((clamp(d.x,0.0,1.0) * (colourout.g + colourout.z)) + colourout.r > groundDepth || colourout.g <= 0.0)){
        colour = c;
        data.z = d.y;
        data.a = ceil(1 - d.x) ;
    
    }
    //data = vec4(texCoords,  d.y, 1);
    //colour = data;
    //colour = vec4(vec3((groundDepth + colourout.z) - groundDepth),1.0);

    //colour = vec4(,1.0);

    vec2 localToMouse = (texCoords - DigPos);
    float ui = 0.0;
    float RfromM = distance(localToMouse, vec2(0));
    if (RfromM < ScreenPixelSize * sizeOfRing && RfromM > ScreenPixelSize * (sizeOfRing - 5) && atan(-localToMouse.x, -localToMouse.y) < (Progress * pi * 2) - pi) {
        colour = vec4(vec3(1), 1);
        ui = 1.0;
    }
    if (RfromM < ScreenPixelSize * (sizeOfRing - 5)) {
        colour = mix(colour, vec4(vec3(1), 1), 0.7f);
        ui = 1.0;
    }

    localToMouse = (texCoords - MousePos);
    RfromM = distance(localToMouse, vec2(0));
    if (RfromM < ScreenPixelSize * (sizeOfRing - 7.5)) {
        colour = mix(colour, vec4(vec3(1), 1), 0.4f);
        ui = 1.0;
    }

    if ((colourout.a <= 0.6 || colourout.g <= 0.0) && c.a < 0.1 && ui <= 0.0) discard;
    else if(colourout.g > 0)data.a = 1.0;
    //colour = colourout;
    // if(c.a > 0.1){
    //     colour = mix(colour,c,c.a);
    //     return;
    // }

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

float tpTex(vec3 normal, vec3 pos, float factor) {

    vec3 blendPercent = normalize(abs(normal));
    float b = (blendPercent.x + blendPercent.y + blendPercent.z);
    blendPercent = blendPercent / vec3(b);

    //vec3 pos = fragmentPos.xyz + u_viewPos;

    float xaxis = noise(pos.yz * factor);
    float yaxis = noise(pos.xz * factor);
    float zaxis = noise(pos.xy * factor);

    return xaxis * blendPercent.x + yaxis * blendPercent.y + zaxis * blendPercent.z;
}

