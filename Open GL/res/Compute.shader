#shader compute
#version 460 core


layout(local_size_x = 256) in;

// Bind vertex buffer as shader storage buffer
layout(r32ui, binding = 0) uniform uimage2D tex;
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

//#define H(p) sin(p)*.5+.5 

void main() {
    uint index = gl_GlobalInvocationID.x;
   
    float r1 = H(index * 1.23);
    float r2 = H(index * 2.34) * (1.0 - r1);
    ivec2 pos;
	pos.x = int(50.0*(r1*triVert.vertices[0]+r2*triVert.vertices[2]+(1-r1-r2)*triVert.vertices[4]));
    pos.y = int(50.0*(r1*triVert.vertices[1]+r2*triVert.vertices[3]+(1-r1-r2)*triVert.vertices[5]));

    imageAtomicAdd(tex, pos, 1);

    /*int r = int(H(index))%3;

    vec2 curvert = vec2(triVert.vertices[2*r],triVert.vertices[2*r+1]);

    vec2 data=vec2((pos.x + curvert.x)/2, (pos.y +curvert.y)/2);*/
}