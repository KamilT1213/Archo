#version 460 core

layout(location = 0)out vec4 colour;
layout(location = 1)out vec4 data;

in vec2 texCoords;

uniform vec2 MousePos;
uniform vec2 DigPos;
uniform vec4 DigStyle;
uniform float isDigging;
uniform float Progress;
uniform float RelicFill;

uniform vec2 u_ScreenSize;
uniform sampler2D u_GroundDepthTexture;
uniform sampler2D u_RelicTexture;
uniform sampler2D u_RelicDataTexture;
uniform sampler2D u_SceneryTexture;
uniform sampler2D u_SceneryDepthTexture;
uniform sampler2D u_SceneryDataTexture;

uniform float u_flip;

float pi = 3.1415;
float sizeOfRing = 5;

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
    vec4 groundData = texture(u_GroundDepthTexture, texCoords); //x: Offset from bottom, y: Thickness, z: Amount Removed, w: 1
    float groundDepth = groundData.x + groundData.y;

    vec4 relicData = texture(u_RelicDataTexture, texCoords); //x: Rarity, y: ID, z: 0, w: 1
    float relicDepth = (relicData.x * (groundData.y + groundData.z)) + groundData.x;

    vec4 sceneryData = texture(u_SceneryDataTexture, texCoords); //x: 1, y: ID, z: 1, w: 1
    float sceneryDepth = 1 - texture(u_SceneryDepthTexture, texCoords).x; 

    colour = vec4(0);
    data = vec4(0);

    if(groundDepth > relicDepth && groundDepth > sceneryDepth){

        vec2 dir = normalize(vec2(1,-1));

        float groundHeightOff = texture(u_GroundDepthTexture, texCoords + (-dir * (1.0 / u_ScreenSize))).r + texture(u_GroundDepthTexture, texCoords + (-dir * (1.0 / u_ScreenSize))).g;



        float n1 = texture(u_GroundDepthTexture, texCoords + (vec2(-1,0) * (1.0 / u_ScreenSize))).r + texture(u_GroundDepthTexture, texCoords + (vec2(-1,0) * (1.0 / u_ScreenSize))).g;
        float n2 = texture(u_GroundDepthTexture, texCoords + (vec2(-1,0) * (1.0 / u_ScreenSize))).r + texture(u_GroundDepthTexture, texCoords + (vec2(-1,0) * (1.0 / u_ScreenSize))).g;
        float n3 = texture(u_GroundDepthTexture, texCoords + (vec2(0,1) * (1.0 / u_ScreenSize))).r + texture(u_GroundDepthTexture, texCoords + (vec2(0,1) * (1.0 / u_ScreenSize))).g;
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
        if(groundDepth < groundHeightOff && distance(groundDepth,groundHeightOff)  > 0.0001 || Dot - (1 - groundDepth) < 0.35){
            //colour -= vec4(0.15, 0.15, 0.15, 0.0) * (1.0 * (1 - groundDepth));
        }

        colour += vec4(vec3(-noiseDis), 0.0);
        if(groundData.z < 0.01) colour.xyz += vec3(hash(floor(texCoords * 1024)) / 20.0);
    }
    else if((relicDepth > groundDepth || groundData.y <= 0) && relicDepth > sceneryDepth){
        colour = texture(u_RelicTexture,texCoords);
        data.z = relicData.y;
        data.x = 1.0 - relicData.x;
        data.y = 0.0;
        data.a = ceil(1 - relicData.x) ;
    }
    else if((sceneryDepth > groundDepth || groundData.y <= 0)&& sceneryDepth > relicDepth){
        colour = texture(u_SceneryTexture,texCoords);
        data.z = sceneryData.y;
        data.y = 1.0;
        data.a = 1.0f;
    }

   
    float groundHeight = texture(u_GroundDepthTexture, texCoords).r + texture(u_GroundDepthTexture, texCoords).g;
    

   



    //float groundDepthAtDig = texture(u_GroundDepthTexture, DigPos).r;



    // vec4 depthMap = texture(u_SceneryTexture, texCoords);

    // if (cHeight < 1 - depthMap.x) {
    //     colour = depthMap;
    // }



   
    


    // vec4 c = texture(u_RelicTexture,texCoords);
    // vec4 d = texture(u_RelicDataTexture,texCoords);
    // if(c.a >= 1 && ((clamp(d.x,-0.1,1.0) * (groundData.g + groundData.z)) + groundData.r > groundHeight || groundData.g <= 0.0)){
    //     colour = c;
    //     data.z = d.y;
    //     data.x = 1.0 - d.x;
    //     data.a = ceil(1 - d.x) ;
    
    // }


    //data = vec4(texCoords,  d.y, 1);
    //colour = data;
    //colour = vec4(vec3((groundDepth + colourout.z) - groundDepth),1.0);

    //colour = vec4(,1.0);
    float ScreenPixelSize = 1.0/min(u_ScreenSize.x, u_ScreenSize.y);
    vec2 localToMouse = (texCoords - DigPos);
    float ui = 0.0;
    float RfromM = distance(localToMouse, vec2(0));
    float size = u_ScreenSize.x/(DigStyle.x * 2);
    if(isDigging > 0.0){
        if (RfromM < ScreenPixelSize * (size + sizeOfRing) && RfromM > ScreenPixelSize * (size) && atan(-localToMouse.x, -localToMouse.y) < (Progress * pi * 2) - pi) {
            colour = mix(colour,vec4(vec3(1), 1),0.7);
            ui = 1.0;
        }
        if (RfromM < ScreenPixelSize * (sizeOfRing - 5)) {
            //colour = mix(colour, vec4(vec3(1), 1), 0.7f);
            //ui = 1.0;
        }
    }
    else{
        if (RfromM < ScreenPixelSize * (size ) && atan(-localToMouse.x, -localToMouse.y) < (Progress * pi * 2) - pi) {
            colour = mix(colour,vec4(vec3(1), 1),0.7);
            ui = 1.0;
        }
        if (RfromM < ScreenPixelSize * (sizeOfRing - 5)) {
            //colour = mix(colour, vec4(vec3(1), 1), 0.7f);
            //ui = 1.0;
        }
    }


    localToMouse = (texCoords - MousePos);
    RfromM = distance(localToMouse, vec2(0));
    if (RfromM < ScreenPixelSize * (sizeOfRing - 7.5)) {
        colour = mix(colour, vec4(vec3(1), 1), 0.4);
        ui = 1.0;
    }



    if (sceneryDepth <= 0 && relicDepth <= 0 && ui <= 0.0 && groundDepth <= 0) discard;
    else if(groundData.g > 0)data.a = 1.0;
    colour.xyz = ((floor(colour.xyz * 20) + 0.5) / 20);
    //colour = vec4(vec3(sceneryDepth), 1.0);
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

