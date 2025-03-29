#version 460 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D u_inputTexture;
uniform float factor;
uniform float allTime;

uniform vec2 u_ScreenSize;

float pi = 3.1415;
float sizeOfRing = 15;

vec4 Samples[9];

float Gaussian[9];


float Edge();
float intensity(vec4 color);
vec3 rgb2hsv(vec3 c);
vec2 hash2(vec2 p);
float noise(in vec2 p);


void main()
{
    vec2 texelSize = 1.0/u_ScreenSize;
    float a = min(u_ScreenSize.x, u_ScreenSize.y);
    float b = max(u_ScreenSize.x, u_ScreenSize.y);
    float c = a / b;

    int offset = 1;
	Samples[0] = texture(u_inputTexture, texCoords + (offset * vec2(-1, 1) * texelSize));
	Samples[1] = texture(u_inputTexture, texCoords + (offset * vec2(0, 1)  * texelSize));
	Samples[2] = texture(u_inputTexture, texCoords + (offset * vec2(1, 1)  * texelSize));
                                                   
 	Samples[3] = texture(u_inputTexture, texCoords + (offset * vec2(-1, 0) * texelSize));
	Samples[4] = texture(u_inputTexture, texCoords + (offset * vec2(0, 0)  * texelSize));
	Samples[5] = texture(u_inputTexture, texCoords + (offset * vec2(1, 0)  * texelSize));
                                                   
 	Samples[6] = texture(u_inputTexture, texCoords + (offset * vec2(-1, -1)* texelSize));
	Samples[7] = texture(u_inputTexture, texCoords + (offset * vec2(0, -1) * texelSize));
	Samples[8] = texture(u_inputTexture, texCoords + (offset * vec2(1, -1) * texelSize));

	Gaussian[0] = 1;
	Gaussian[1] = 2;
	Gaussian[2] = 1;
	Gaussian[3] = 2;
	Gaussian[4] = 4;
	Gaussian[5] = 2;
	Gaussian[6] = 1;
	Gaussian[7] = 2;
	Gaussian[8] = 1;

	vec4 total;

    if (Edge() >= 0.0) {
        for (int i = 0; i < 9; i++) {
            total += Samples[i] * Gaussian[i];
        }
        total /= 16.0f;
    }
    
    total = mix(Samples[4], total, clamp(Edge(),0,1));
    total.xyz = ((floor(total.xyz * 20) + 0.5) / 20);
    float bTotal;
    if (Samples[4].a <= 0) {
        for (int i = 0; i < 5; i++) {
            bTotal += noise((texCoords + vec2(i) + vec2(sin((allTime*3) + (texCoords.y * 5)) / 100, allTime/10)) * (i + 1) * 42.532);
        }
        bTotal = ((bTotal / 5) + 1) / 2.0;
        bTotal = ((floor(bTotal * 20) + 0.5) / 20);

        vec2 normedTex = texCoords;
        vec2 dist = vec2(normedTex.x - 0.5f, normedTex.y - 0.5f) ;

        if (u_ScreenSize.x > u_ScreenSize.y) {
            dist.x *= (u_ScreenSize.x / u_ScreenSize.y);
        }
        else {
            dist.y *= (u_ScreenSize.y / u_ScreenSize.x);
        }
        

        float d = c;
       /* d *= 0.5;
        d = d / b;
        d *= 2.0;*/

        float s = 1;

        dist *= s;
        

        float distance = (max(abs(dist.x), abs(dist.y)) - (d * s));
        bTotal += clamp(((distance) * 2),-1.0,0.3);
        total =  vec4(vec3(0.3,bTotal,0.6), 1.0);
    }
            
    colour = total;// vec4(vec3(rgb2hsv(Samples[4].rgb).z), 1); //vec4(vec3(Edge()), 1);//

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

float Edge() {
    // get samples around pixel


    float GaussianDiff[9];
    GaussianDiff[0] = rgb2hsv(Samples[0].rgb).y;
    GaussianDiff[1] = rgb2hsv(Samples[1].rgb).y;
    GaussianDiff[2] = rgb2hsv(Samples[2].rgb).y;
    GaussianDiff[3] = rgb2hsv(Samples[3].rgb).y;
    GaussianDiff[4] = rgb2hsv(Samples[4].rgb).y;
    GaussianDiff[5] = rgb2hsv(Samples[5].rgb).y;
    GaussianDiff[6] = rgb2hsv(Samples[6].rgb).y;
    GaussianDiff[7] = rgb2hsv(Samples[7].rgb).y;
    GaussianDiff[8] = rgb2hsv(Samples[8].rgb).y;

    float GaussianDiffFactor[9];
    GaussianDiffFactor[0] = -1;
    GaussianDiffFactor[1] = -2;
    GaussianDiffFactor[2] = -1;

    GaussianDiffFactor[3] = -2;
    GaussianDiffFactor[4] = 12;
    GaussianDiffFactor[5] = -2;

    GaussianDiffFactor[6] = -1;
    GaussianDiffFactor[7] = -2;
    GaussianDiffFactor[8] = -1;

    float total;
    for (int i = 0; i < 9; i++) {
        total += GaussianDiff[i] * GaussianDiffFactor[i];
    }

    return (total);
    /*mat3 xSobel;
    xSobel[0][0] = 1;
    xSobel[0][1] = 2;
    xSobel[0][2] = 1;
    xSobel[1][0] = 0;
    xSobel[1][1] = 0;
    xSobel[1][2] = 0;
    xSobel[2][0] = -1;
    xSobel[2][1] = -2;
    xSobel[2][2] = -1;

    mat3 ySobel;
    ySobel[0][0] = -1;
    ySobel[0][1] = 0;
    ySobel[0][2] = 1;
    ySobel[1][0] = -2;
    ySobel[1][1] = 0;
    ySobel[1][2] = 2;
    ySobel[2][0] = -1;
    ySobel[2][1] = 0;
    ySobel[2][2] = 1;*/


    // Sobel masks (see http://en.wikipedia.org/wiki/Sobel_operator)
    //        1 0 -1     -1 -2 -1
    //    X = 2 0 -2  Y = 0  0  0
    //        1 0 -1      1  2  1

    // You could also use Scharr operator:
    //        3 0 -3        3 10   3
    //    X = 10 0 -10  Y = 0  0   0
    //        3 0 -3        -3 -10 -3

    

    //float x = 0;// tleft + 2.0 * left + bleft - tright - 2.0 * right - bright;
    //float y = 0;// -tleft - 2.0 * top - tright + bleft + 2.0 * bottom + bright;

    //for (int i = 0; i < 3; i++) {
    //    for (int j = 0; j < 3; j++) {
    //        x += xSobel[i][j] * GaussianDiff[i][j];
    //        y += ySobel[i][j] * GaussianDiff[i][j];
    //    }
    //}

    //float r = (((atan(x, y) * 2.0) + 3.1415)) / (4.0 * 3.1415);
    //float n = sqrt((x * x) + (y * y));
    //if (n < 0.001){
    //    return vec3(0, 0, 0);
    //}

    //return vec3(-x / n, y / n, n);

}

float intensity(in vec4 color) {
    //color = ((floor(color) + 0.5));
     return sqrt((color.x * color.x) + (color.y * color.y) + (color.z * color.z));
 
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
//
//vec3 hsv2rgb(vec3 c)
//{d
//    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
//}
//
//vec2 rotate2D(vec2 a, float t) {
//    float c = cos(t);
//    float s = sin(t);
//    return vec2((c * a.x) - (s * a.y), (s * a.x) + (c * a.y));
//}
