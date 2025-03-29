#version 460 core
layout(local_size_x = 32, local_size_y = 32) in;
layout(binding = 0, rgba16f) uniform image2D GroundImg;
layout(binding = 1, rgba16f) uniform image2D groundNormalImg;
//layout(binding = 1) uniform vec3[]

vec4 Slerp(vec4 p0, vec4 p1, float t);

void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);


	float displacement0 = (imageLoad(GroundImg, pixel_coords + ivec2(-1, 0)).x) * 25;
	float displacement1 = (imageLoad(GroundImg, pixel_coords + ivec2(1, 0)).x) * 25;
	float displacement2 = (imageLoad(GroundImg, pixel_coords + ivec2(0, 1)).x) * 25;
	float displacement3 = (imageLoad(GroundImg, pixel_coords + ivec2(0, -1)).x) * 25;

	float dir1 = displacement0 - displacement1;
	float dir2 = displacement2 - displacement3;
	vec3 norm = normalize(vec3(dir1, 0.5, dir2));

	//
	float displacement4 = (imageLoad(GroundImg, pixel_coords + ivec2(-1, -1)).x) * 25;
	float displacement5 = (imageLoad(GroundImg, pixel_coords + ivec2(1, 1)  ).x) * 25;
	float displacement6 = (imageLoad(GroundImg, pixel_coords + ivec2(-1, 1) ).x) * 25;
	float displacement7 = (imageLoad(GroundImg, pixel_coords + ivec2(1, -1) ).x) * 25;



	float dir3 = displacement4 - displacement5;
	float dir4 = displacement6 - displacement7;
	vec3 norm2 = normalize(vec3(dir3, 0.5, dir4));


	//vec4 Col = Slerp(vec4(norm, 1.0), vec4(norm2, 1.0), 0.5);
	vec4 Col = vec4(norm, 1.0);
	if (isnan(Col).x) Col = imageLoad(groundNormalImg, pixel_coords + ivec2(1,0));
	//Col = vec4(vec2(pixel_coords.x/ 4096.0,pixel_coords.y / 4096.0),0,1);
	imageStore(groundNormalImg, pixel_coords, Col);
}

vec4 Slerp(vec4 p0, vec4 p1, float t)
{
	float dotp = dot(normalize(p0), normalize(p1));
	if ((dotp > 0.9999) || (dotp < -0.9999))
	{
		if (t <= 0.5)
			return p0;
		return p1;
	}
	float theta = acos(clamp(dotp, -1, 1));
	if (theta == 0) theta = 0.001;
	vec4 P = ((p0 * sin((1 - t) * theta) + p1 * sin(t * theta)) / sin(theta));
	P.w = 1;
	return P;
}