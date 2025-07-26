#shader compute
#version 460 core


layout(local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

// Bind vertex buffer as shader storage buffer
layout(rgba8, binding = 0) uniform image2D tex;
/*layout(std430, binding = 1) buffer triangle {
    float vertices[6];
} triVert;*/
struct TriVert {
    float vertices[6];
};

TriVert triVert = TriVert(
    float[6](
        -0.5, -0.5,
         0.5, -0.5,
         0.0,  0.5
    )
);
#define H(p) fract(sin(float(p) * 12345.6789) * 43758.5453)

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

// Get a float between 0 and 1
float randomFloat(uint state)
{
    return fract(sin(state) * 43758.5453);
    //return wang_hash(state) / float(0xffffffff);
}
//#define H(p) sin(p)*.5+.5 

void main() {
    uvec2 index = gl_GlobalInvocationID.xy;
    ivec2 pos;
 //    uint state = index.x+8*index.y;
   
 //    // float r1 = H(index * 1.23);
 //    // float r2 = H(index * 2.34) * (1.0 - r1); 
 //    float r1 = randomFloat(state++);
 //    float r2 =  randomFloat(state+int(r1*123654));
 //    
	// pos.x = int(50.0*(r1*triVert.vertices[0]+r2*triVert.vertices[2]+(1-r1-r2)*triVert.vertices[4]));
 //    pos.y = int(50.0*(r1*triVert.vertices[1]+r2*triVert.vertices[3]+(1-r1-r2)*triVert.vertices[5]));
 //    pos.x = int(r1*50.0);
 //    pos.y = int(r2*50.0);
    
    pos.x = int(index.x);
    pos.y = int(index.y);

    imageStore(tex, pos, vec4(1.0,1.0,1.0,1.0));

    /*int r = int(H(index))%3;

    vec2 curvert = vec2(triVert.vertices[2*r],triVert.vertices[2*r+1]);

    vec2 data=vec2((pos.x + curvert.x)/2, (pos.y +curvert.y)/2);*/
}