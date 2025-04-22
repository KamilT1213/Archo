#version 460 core
layout(local_size_x = 12, local_size_y = 12) in;
layout(binding = 0, rgba16f) uniform image2D ImgIn;

struct SeedingPoint {
	vec4 position;
};

layout(std430, binding = 3) buffer ssbo0
{
	SeedingPoint seedingPoints[];
};

uniform vec2 Size;

void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 groupID = ivec2(gl_WorkGroupID);
	int index = groupID.x + (int(gl_NumWorkGroups.x) * groupID.y);


	vec4 currentPos = seedingPoints[index].position;

	//if(currentPos.x < 0){
	
		float currentSize = Size.x / (currentPos.w * 2);
		vec4 sam = imageLoad(ImgIn, pixel_coords);
		float dist = sam.y * (Size.x / 8.0);

		if(dist >= currentSize && sam.a > 0){
			vec2 held = pixel_coords;
			held.y = Size.y - held.y;
			seedingPoints[index].position = vec4(vec2(held), dist, currentPos.w);
		}
		else{
			seedingPoints[index].position = vec4(vec2(-100), dist, currentPos.w);
		}
		
	//}
	//seedingPoints[index].position = vec4(vec2(gl_WorkGroupID.xy) * 12,1, currentPos.w);

}
